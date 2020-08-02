#include "kernel.h"

#include "states/init.c"

KernelStateInfo* kernel_info;

/*
 * The kernel entrypoint
 */
void kernel_main(uint8_t* boot_info) {
	init_serial();

	uint32_t total_size = *((uint32_t*) boot_info);

	// Skip total_size and reserved
	int i = 8;

	while (i < total_size) {
		uint32_t type = *((uint32_t*) (boot_info + i));
		serial_putchar('0' + type);
		uint32_t size = *((uint32_t*) (boot_info + i + 4));
		if (type != 8) {
			i += (size+7)&~7;
		} else {
			int* fb_addr = (int*) *((uint64_t*) (boot_info + i + 8));
			for(int i = 0; i < 1000*100;++i)fb_addr[i] = 0xffffffff;
			asm volatile("ud2");
		}
	}

	// Main kernel loop
	while (true) {
		cli();
		hlt();
	}
}

/*
 * A single L0 state exectution and transition
 */
void l0_tick(void) {
	switch (kernel_info->l0_state) {
	case L0Entering:
		kernel_info->l1_state->enter_function();
		kernel_info->l0_state = L0Next;
		break;
	case L0Tick:
		kernel_info->l1_state->tick_function();
		kernel_info->l0_state = L0Next;
		break;
	case L0Next:
		
	default:
		break;
	}
}
