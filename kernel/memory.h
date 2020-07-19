// Based on https://github.com/ZeusWPI/TABS/blob/master/kernel/memory.c by @mcbloch

#ifndef KERNEL_MEMORY_H
#define KERNEL_MEMORY_H

#include <stddef.h>
#include <stdbool.h>

#include "terminal.h"

#define MEMORY_START 0x200000
#define MEM_SIZE_WIDTH 3

// The tag that gets added to the beginning of each allocated block in memory
typedef struct page_tag {
    size_t size;
    struct page_tag *prev;
    struct page_tag *next;
} page_tag;

// Allocate a block of the given size
void* alloc(size_t size);

// Free a block
void free(void *data);

// Dump the memory allocation
void print_memory(void);

// Initialize the memory management
void memory_init(void);
#endif //KERNEL_MEMORY_H