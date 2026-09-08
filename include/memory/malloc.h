#ifndef __MALLOC__
#define __MALLOC__

#include <stddef.h>
// #include <stdbool.h>
#include <stdint.h>
typedef struct
{
    size_t size;
    _Bool free;
    struct header* next;
} header;

extern char kernel_end;
void heap_init(void);
void* kmalloc(size_t size);

#endif