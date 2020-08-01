#ifndef KERNEL_STATE_H
#define KERNEL_STATE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "terminal.h"
#include "inline_asm.h"
#include "memory.h"

#define transition_t uint32_t
#define state_t uint32_t

// ----- LEVEL 0 -----

/*
 * The lowest-level state
 */
enum L0State {
    L0Entering = 0,   /* Entering a new level 1 state, called once */
    L0Next = 1,       /* Finding a new level 1 state, not state-dependent except for the LTT, interrupts enabled */
    L0Tick = 2,       /* Ticking the level 1 state, called when NEXT decides there should be no state transition */
    L0Invalid = -1,
};

// ----- LEVEL 1 -----

/*
 * Defines a single transition
 */ 
typedef struct L1TransitionStruct {
    transition_t id;
    state_t new_state;
    struct L1TransitionStruct* next;
} L1Transition;

/*
 * Defines a Transition Table (global or local)
 */
typedef struct {
    int num_entries;
    L1Transition* first_entry;
} TransitionTable;

/*
 * A level 1 state
 */
typedef struct {
    state_t id;
    void (*enter_function)(void);
    void (*tick_function)(void);
    TransitionTable* ltt;
} L1State;

// ----- Kernel -----

/*
 * All the state-info the kernel needs
 */
typedef struct {
    TransitionTable* gtt;
    enum L0State l0_state;
    L1State* l1_state;
} KernelStateInfo;

void kernel_main(void);
void l0_tick(void);

#endif //KERNEL_STATE_H
