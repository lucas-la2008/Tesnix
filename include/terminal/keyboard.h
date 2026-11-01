#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <lib/kstdint.h>

void keyboard_scancode(uint8_t key, char* out);
char keyboard_char(uint8_t scancode);

#endif