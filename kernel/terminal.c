#include "terminal.h"

Terminal __global_terminal = {
	.width = 80,
	.height = 25,
	.row = 0,
	.column = 0,
	.color = 0x70,
	.buffer = (uint16_t*) 0xB8000,
};

Terminal* terminal_new() {
	// TODO fix after alloc is implemented
	__global_terminal.width = 80;
	__global_terminal.height = 25;
	__global_terminal.row = 0;
	__global_terminal.column = 0;
	__global_terminal.color = 0x70;
	__global_terminal.buffer = (uint16_t*) 0xB8000;
	return &__global_terminal;
}

Terminal* terminal_aquire(void);
void terminal_free(Terminal** terminal);

void printk(Terminal terminal, const char* message);
void terminal_putchar(Terminal terminal, char character);
void terminal_move(Terminal terminal, size_t row, size_t column);
void terminal_setcolor(Terminal terminal, uint8_t color);

