#ifndef KERNEL_SCREEN_H
#define KERNEL_SCREEN_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
	size_t width;
	size_t height;
	size_t row;
	size_t column;
	uint8_t color;
	const uint16_t* buffer;
} Terminal;

Terminal* terminal_new(void);
Terminal* terminal_aquire(void);
void terminal_free(Terminal** terminal);

void printk(Terminal terminal, const char* message);
void terminal_putchar(Terminal terminal, char character);
void terminal_move(Terminal terminal, size_t row, size_t column);
void terminal_setcolor(Terminal terminal, uint8_t color);

#endif //KERNEL_SCREEN_H
