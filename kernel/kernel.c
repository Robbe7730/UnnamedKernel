#include "kernel.h"

#include "states/init.c"

KernelStateInfo* kernel_info;

/*
 * The kernel entrypoint
 */
void kernel_main(void) {
	// memory_init();

	// Setup kernel_info with no GTT and being in INIT/E state
	// kernel_info = alloc(sizeof(KernelStateInfo));
	// kernel_info->gtt = NULL;
	// kernel_info->l0_state = L0Entering;
	// kernel_info->l1_state = STATE_INIT;

	printk("Testen");

	// Main kernel loop
	while (true) {
		sti();
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
