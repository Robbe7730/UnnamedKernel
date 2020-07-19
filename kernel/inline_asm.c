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