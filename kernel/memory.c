#include "memory.h"

page_tag* __global_start;

void memory_init(void) {
    __global_start = MEMORY_START;
    __global_start->next = NULL;
    __global_start->prev = NULL;
    __global_start->size = 0;
}

void free(void* data) {
    // As the user has a pointer to the start of the data, we need to subtract the size of the page_tag to get the original page_tag
    page_tag* data_tag = (page_tag*) ((void*) data) - sizeof(page_tag);

    // Correct the pointers for the previous and next blocks
    data_tag->prev->next = data_tag->next;
    data_tag->next->prev = data_tag->prev;

    data_tag->next = NULL;
    data_tag->prev = NULL;
    data_tag->size = 0;
}

void* alloc(size_t size) {
    page_tag* current_tag = __global_start;

    while (current_tag->next != NULL) {
        page_tag* next_tag = current_tag->next;

        // +-------------+-------------------+---------------------+----------+
        // | current_tag | current_tag->size | possible free space | next_tag |
        // +-------------+-------------------+---------------------+----------+
        // Calculate the space between two consecutive blocks
        uint32_t free_space_between = (uint32_t) next_tag - (uint32_t) current_tag - sizeof(page_tag) - current_tag->size;
        if (free_space_between >= (size + sizeof(page_tag))) {
            page_tag* new_page_tag = (page_tag*)((void*) current_tag + sizeof(page_tag) + current_tag->size);
            new_page_tag->size = size;
            new_page_tag->prev = current_tag;
            new_page_tag->next = current_tag->next;
            current_tag->next = new_page_tag;
            new_page_tag->next->prev = new_page_tag;

            return new_page_tag + sizeof(page_tag);
        }

        current_tag = next_tag;
    }

    // We have reached the end of the line, create a new tag and return that one
    if (current_tag->next == NULL) {
        current_tag->next = (void*) current_tag + sizeof(page_tag) + current_tag->size;
        current_tag->next->size = size;
        current_tag->next->next = NULL;
        current_tag->next->prev = current_tag;

        return current_tag->next + sizeof(page_tag);
    }

    return NULL;
}

void print_memory() {
    printk("____________________\n");
    printk(" ===  MEM DUMP  === \n");
    printk("--------------------\n");
    printk("Page tag size:    %d bytes\n", sizeof(page_tag));
    printk("=> idx: [page_tag_address] [data_address (size)]\n\n");
    page_tag *curr_page = __global_start;
    int i = 0;
    while (curr_page != NULL) {
        printk("%d: [%x] [%x (%d)]",
              i,
              curr_page,
              (curr_page + 1), curr_page->size);
        printk("\n");

        void *empty_start = (void *) curr_page + sizeof(page_tag) + curr_page->size;
        if (empty_start + sizeof(page_tag) < (void *) curr_page->next) {
            size_t memory_left = (void *) curr_page->next -
                                 (empty_start + sizeof(page_tag));

            printk("_: free space      (%d + %d)\n",
                  sizeof(page_tag),
                  memory_left
            );
        } else if (empty_start < (void *) curr_page->next) {
            size_t memory_left = (void *) curr_page->next - empty_start;

            printk("_: not enough room      (%d)\n",
                  memory_left);
        }


        curr_page = curr_page->next;
        i += 1;
    }
    printk("____________________\n");
}