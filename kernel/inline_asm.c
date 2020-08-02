#include "inline_asm.h"

void cli(void) {
    asm("cli");
}

void sti(void) {
    asm("sti");
}

void hlt(void) {
    asm("hlt");
}

void outb(uint16_t port, uint8_t val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}
