#include "memory/malloc.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

uintptr_t heap_start;
uintptr_t heap_end;
#define HEAP_SIZE (16*1024*1024)

void heap_init(void)
{
    heap_start = ((uintptr_t)&kernel_end + 7) & ~7;
    heap_end = heap_start + HEAP_SIZE;

    header *ptr = (header *)heap_start;

    ptr->free = true;
    ptr->next = NULL;
    ptr->size = HEAP_SIZE - sizeof(header);
}

void* kmalloc(size_t size)
{
    if (size == 0)
    {
        return NULL;
    }
    header* ptr = (header *)heap_start;

    bool found = false;
    while((uintptr_t)ptr < heap_end)
    {
        if (((uintptr_t)ptr+sizeof(header) < heap_end) && ptr->size >=size)
        {
            found = true;
            break;
        }
        ptr = (header*)((uint8_t*)ptr+sizeof(header)+ptr->size);
    }
    if (!found)
    {
        return NULL;
    }
    
    header *new = (header *)((uint8_t*)ptr + sizeof(header)+size);
    new->free = true;
    new->size = heap_end-((uintptr_t)new + sizeof(header));
    new->next = NULL;
    
    ptr->size = size;
    ptr->free = false;
    ptr->next = (struct header*)new;
    return (void *)((uintptr_t)ptr + sizeof(header));
}