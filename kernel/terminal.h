#ifndef KERNEL_SCREEN_H
#define KERNEL_SCREEN_H

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

typedef struct {
	size_t width;
	size_t height;
	size_t row;
	size_t column;
	uint8_t color;
	uint16_t* buffer;
} Terminal;

// --- Terminal struct operations ---

// Create a new Terminal
Terminal* terminal_new(void);

// Get the current Terminal
Terminal* terminal_get(void);

// Free a Terminal, setting the Terminal* to NULL
void terminal_free(Terminal** terminal);

// --- Helper functions ---

// Create a uint16_t entry to write to the VGA buffer
uint16_t vga_entry(Terminal* terminal, unsigned char character);

// Clears te screen
void terminal_clear(Terminal* terminal);

// Write one character to the terminal
void terminal_putchar(Terminal* terminal, unsigned char character);

// Move the Terminal a certain number of rows/cols
void terminal_move(Terminal* terminal, size_t row, size_t column);

// Set the color of the Terminal
void terminal_setcolor(Terminal* terminal, uint8_t color);

// --- (External) utility functions ---

/**
 * fmt is a string with specifiers where variables can be inserted.
 *
 * ex: ("Hello %s", "world") -> "Hello world"
 *
 * Specifiers are
 *   - %c: character
 *   - %d: digit (integer)
 *   - %x: hexadecimal value. Can be used to print pointers
 *   - %b: binary value. Digits with base 2
 * @param fmt Formatter string
 * @param ... Variable amount of arguments to be inserted
 */
void printk(const char *fmt, ...);

// Clears the screen
void cls(void);

// Make a string from a number
char* itoa(unsigned int value, char* result, int base);

#endif //KERNEL_SCREEN_H
