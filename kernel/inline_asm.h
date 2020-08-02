#ifndef KERNEL_INLINE_ASM_H
#define KERNEL_INLINE_ASM_H

#include <stdint.h>

// Clears the interrupt flag (disables interrupts)
void cli(void);

// Sets the intterupt flag (enables interrupts)
void sti(void);

// Halt (and don't catch fire)
void hlt(void);

// Output a byte to a port
void outb(uint16_t port, uint8_t val);

// Read a byte from a port
uint8_t inb(uint16_t port);

#endif //KERNEL_INLINE_ASM_H
