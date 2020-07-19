#ifndef KERNEL_INLINE_ASM_H
#define KERNEL_INLINE_ASM_H

// Clears the interrupt flag (disables interrupts)
void cli(void);

// Sets the intterupt flag (enables interrupts)
void sti(void);

// Halt (and don't catch fire)
void hlt(void);

#endif //KERNEL_INLINE_ASM_H