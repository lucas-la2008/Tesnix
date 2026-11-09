#ifndef __KMALLOC_H__
#define __KMALLOC_H__

#include <lib/kstddef.h>
#include <lib/kstdbool.h>
#include <lib/kstdint.h>

#define HEAP_SIZE (16*1024*1024)

typedef struct header header;

struct header
{
    size_t size;
    header* next;
    int64_t magic;
    bool free;
};

extern char kernel_end;
void heap_init(void);
void* kmalloc(size_t size);
void kfree(void* ptr);

#endif