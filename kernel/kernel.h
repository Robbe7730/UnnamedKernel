#ifndef KERNEL_STATE_H
#define KERNEL_STATE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "terminal.h"
#include "inline_asm.h"
#include "memory.h"
 
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This kernel will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif

#define transition_t uint32_t
#define state_t uint32_t

// ----- LEVEL 0 -----

/*
 * The lowest-level state
 */
typedef enum {
    ENTERING,   /* Entering a new level 1 state, called once */
    NEXT,       /* Finding a new level 1 state, not state-dependent except for the LTT, interrupts enabled */
    TICK,       /* Ticking the level 1 state, called when NEXT decides there should be no state transition */
    INVALID,
} L0State;

// ----- LEVEL 1 -----

/*
 * Defines a single transition
 */ 
typedef struct {
    transition_t id;
    state_t new_state;
} L1Transition;

/*
 * Defines a Transition Table (global or local)
 */
typedef struct {
    int num_entries;
    L1Transition* entries;
} TransitionTable;

/*
 * A level 1 state
 */
typedef struct {
    state_t id;
    void (*enter_function)(void*);
    void (*tick_function)(void*);
    TransitionTable* ltt;
} L1State;

// ----- Kernel -----

/*
 * All the state-info the kernel needs
 */
typedef struct {
    TransitionTable* gtt;
    L0State* l0_state;
    L1State* l1_state;
} KernelStateInfo;

void kernel_main(void);

#endif //KERNEL_STATE_H