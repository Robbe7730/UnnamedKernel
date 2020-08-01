#include "terminal.h"

Terminal* __global_terminal;

Terminal* terminal_new() {
	__global_terminal = alloc(sizeof(Terminal));
	__global_terminal->width = 80;
	__global_terminal->height = 25;
	__global_terminal->row = 0;
	__global_terminal->column = 0;
	__global_terminal->color = 0x07;
	__global_terminal->buffer = (uint16_t*) 0xB8000;
	return __global_terminal;
}

Terminal* terminal_get(void) {
	return __global_terminal;
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
	free(*terminal);
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

void terminal_writestring(Terminal* terminal, const char* string) {
	int i = 0;
	while (string[i] != '\0') {
		terminal_putchar(terminal, string[i]);
		i++;
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

char* itoa(unsigned int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

void printk(const char *fmt, ...) {
	Terminal* terminal = terminal_get();
    va_list argp;
    char fmtbuf[256];

	int i;
	char* s;

    va_start(argp, fmt);

    for (const char* p = fmt; *p != '\0'; p++)
        if (*p != '%') {
            terminal_putchar(terminal, *p);
        } else {
            switch (*++p) {
                case 'c':
                    i = va_arg(argp, int);

                    terminal_putchar(terminal, (char) i);
                    break;
                case 'd':
                    i = va_arg(argp, int);
                    s = itoa(i, fmtbuf, 10);
                    terminal_writestring(terminal, s);
                    break;
                case 's':
                    s = va_arg(argp, char *);
                    terminal_writestring(terminal, s);
                    break;
                case 'x':
                    terminal_writestring(terminal, "0x");

                    i = va_arg(argp, int);
                    s = itoa(i, fmtbuf, 16);
                    terminal_writestring(terminal, s);
                    break;
                case 'b':
                    terminal_writestring(terminal, "0b");

                    i = va_arg(argp, int);
                    s = itoa(i, fmtbuf, 2);
                    terminal_writestring(terminal, s);
                    break;
                case '%':
                    terminal_putchar(terminal, '%');
                    break;
            }
        }

    va_end(argp);
}

void cls(void) {
	Terminal* terminal = terminal_get();
	terminal_clear(terminal);
}
