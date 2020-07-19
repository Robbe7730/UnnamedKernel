#include "terminal.h"

Terminal __global_terminal = {
	.width = 80,
	.height = 25,
	.row = 0,
	.column = 0,
	.color = 0x07,
	.buffer = (uint16_t*) 0xB8000,
};

Terminal* terminal_new() {
	// TODO fix when alloc is implemented
	__global_terminal.width = 80;
	__global_terminal.height = 25;
	__global_terminal.row = 0;
	__global_terminal.column = 0;
	__global_terminal.color = 0x07;
	__global_terminal.buffer = (uint16_t*) 0xB8000;
	return &__global_terminal;
}

Terminal* terminal_get(void) {
	return &__global_terminal;
}

uint16_t vga_entry(Terminal* terminal, unsigned char character) {
	return (uint16_t) character | (uint16_t) terminal->color << 8;
}

void terminal_clear(Terminal* terminal) {
	for (size_t i = 0; i < terminal->width; i++) {
		for (size_t j = 0; j < terminal->height; j++) {
			terminal_putchar(terminal, ' ');
		}
	}
}

void terminal_free(Terminal** terminal) {
	// TODO fix when free is implemented
	*terminal = NULL;
}

void terminal_putchar(Terminal* terminal, unsigned char character) {
	if (character == '\n') {
		terminal_move(terminal, 1, -(terminal->column));
	} else {
		const size_t index = terminal->row * terminal->width + terminal->column;
		terminal->buffer[index] = vga_entry(terminal, character);
		terminal_move(terminal, 0, 1);
	}
}

void terminal_move(Terminal* terminal, size_t rows, size_t columns) {
	terminal->column += columns;
	if (terminal->column >= terminal->width) {
		size_t buf = terminal->column;
		terminal->column = buf % terminal->width;
		terminal->row += buf / terminal->width;
	}
	terminal->row += rows;
	if (terminal->row >= terminal->height) {
		// TODO: scroll screen
		terminal->row = 0;
		terminal->column = 0;
	}
}

void terminal_setcolor(Terminal* terminal, uint8_t color) {
	terminal->color = color;
}

void printk(const char* message) {
	Terminal* terminal = terminal_get();
	int i = 0;
	while (message[i] != '\0') {
		terminal_putchar(terminal, message[i]);
		i++;
	}
}

void cls(void) {
	Terminal* terminal = terminal_get();
	terminal_clear(terminal);
}