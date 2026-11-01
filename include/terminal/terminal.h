#ifndef __VGA_TERMINAL_H__
#define __VGA_TERMINAL_H__

#include <lib/kstdbool.h>
#include <lib/kstdint.h>
#include <lib/kstddef.h>

void terminal_initialize(void);
void terminal_setcolor(uint8_t color);
void terminal_scroll();
void vga_putchar(char c);
void vga_write(const char *info, size_t size);
void vga_writestring(const char *string);
void detect_command();
void update_cursor(size_t y, size_t x);
void clear(bool isPrompt);
void clean_line(void);
void clean_command(void);

#endif