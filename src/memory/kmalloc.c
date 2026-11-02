#include <lib/kstddef.h>
#include <lib/kstdint.h>
#include <lib/kstdbool.h>
#include "hash/hash.h"
#include "memory/kmalloc.h"

uintptr_t heap_start;
uintptr_t heap_end;
int64_t MAGIC;
#define HEAP_SIZE (16*1024*1024)

void heap_init(void)
{
    heap_start = ((uintptr_t)&kernel_end + 7) & ~7;
    heap_end = heap_start + HEAP_SIZE;
    MAGIC = hash(heap_start+heap_end);
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
        if (((uintptr_t)ptr+sizeof(header) < heap_end) && ptr->size >=size && ptr->free)
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
    ptr->next = new;
    return (void *)((uintptr_t)ptr + sizeof(header));
}

void kfree(void* ptr){
    if (ptr == NULL){
        return;
    }
    header* headerptr = (header*)((uintptr_t)ptr - sizeof(header));
    if (headerptr->magic != MAGIC){
    }
    if (headerptr->free){
        return;
    }
    headerptr->free = true;
    header* new  = (header*)((uintptr_t)headerptr + sizeof(header) + headerptr->size);
    while (new->free && (uintptr_t)new+sizeof(header) < heap_end){
        if (new->magic != MAGIC){
            
        }
        headerptr->size += new->size + sizeof(header);
        headerptr->next = new->next;
        new  = (header*)((uintptr_t)headerptr + sizeof(header) + headerptr->size);
    }
}