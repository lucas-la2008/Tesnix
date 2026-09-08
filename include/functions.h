#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

size_t kstrlen(const char *str);
size_t kstrcmp(const char* str1, const char* str2);
void toStr(size_t v,char* buffer);
void toHex(size_t value, char *out);
#endif