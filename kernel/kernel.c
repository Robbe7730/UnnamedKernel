#include "kernel.h"

void kernel_main(void) {
	memory_init();

	cls();
	printk("Hello world!\n");

	void* to_be_cleared = alloc(7000);
	alloc(2000);
	alloc(3000);
	alloc(4000);
	alloc(5000);
	free(to_be_cleared);
	void* last = alloc(6000);

	print_memory();

	// Infinite loop
	cli();
	while(true) {
		hlt();
	}
}
