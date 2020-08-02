#ifndef KERNEL_SERIAL_H
#define KERNEL_SERIAL_H

#include "inline_asm.h"
#include "util.h"

#include <stdarg.h>

#define PORT 0x3f8   /* COM1 */
 
void init_serial(void);

int is_transmit_empty(void);

void serial_putchar(char a);

void serial_writestring(const char* str);

#endif //KERNEL_SERIAL_H
