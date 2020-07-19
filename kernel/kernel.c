#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "terminal.h"
#include "inline_asm.h"
#include "memory.h"
 
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

void kernel_main(void);

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
