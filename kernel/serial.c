#include "serial.h"

void init_serial() {
  outb(PORT + 1, 0x00); // Disable all interrupts
  outb(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
  outb(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
  outb(PORT + 1, 0x00); //                  (hi byte)
  outb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
  outb(PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
  outb(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

int is_transmit_empty() { return inb(PORT + 5) & 0x20; }

void serial_putchar(char a) {
  while (is_transmit_empty() == 0)
    ;

  outb(PORT, a);
}

void serial_writestring(const char *string) {
  int i = 0;
  while (string[i] != '\0') {
    serial_putchar(string[i]);
    i++;
  }
}

void serial_printk(const char *fmt, ...) {
  va_list argp;
  char fmtbuf[256];

  int i;
  char *s;

  va_start(argp, fmt);

  for (const char *p = fmt; *p != '\0'; p++)
    if (*p != '%') {
      serial_putchar(*p);
    } else {
      switch (*++p) {
      case 'c':
        i = va_arg(argp, int);

        serial_putchar((char)i);
        break;
      case 'd':
        i = va_arg(argp, int);
        s = itoa(i, fmtbuf, 10);
        serial_writestring(s);
        break;
      case 's':
        s = va_arg(argp, char *);
        serial_writestring(s);
        break;
      case 'x':
        serial_writestring("0x");

        i = va_arg(argp, int);
        s = itoa(i, fmtbuf, 16);
        serial_writestring(s);
        break;
      case 'b':
        serial_writestring("0b");

        i = va_arg(argp, int);
        s = itoa(i, fmtbuf, 2);
        serial_writestring(s);
        break;
      case '%':
        serial_putchar('%');
        break;
      }
    }

  va_end(argp);
}
