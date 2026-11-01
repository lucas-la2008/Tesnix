#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include <lib/kstdint.h>
#include <lib/kstdbool.h>
#include <lib/kstddef.h>

size_t kstrlen(const char *str);
size_t kstrcmp(const char* str1, const char* str2);
void toStr(size_t v,char* buffer);
void toHex(size_t value, char *out);
#endif