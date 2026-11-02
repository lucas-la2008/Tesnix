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
    // Align the heap memory start to an 8-byte boundary
    heap_start = ((uintptr_t)&kernel_end + 7) & ~7;
    heap_end = heap_start + HEAP_SIZE;
    MAGIC = hash(heap_start+heap_end);
    header *ptr = (header *)heap_start;

    ptr->free = true;
    ptr->next = NULL;
    ptr->size = HEAP_SIZE - sizeof(header);
    ptr->magic = MAGIC;
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
    new->magic = MAGIC;
    
    ptr->size = size;
    ptr->free = false;
    ptr->next = new;
    ptr->magic = MAGIC;
    return (void *)((uintptr_t)ptr + sizeof(header));
}

void kfree(void* ptr){
    if (ptr == NULL){
        return;
    }
    header* headerptr = (header*)((uintptr_t)ptr - sizeof(header));
    if (headerptr->magic != MAGIC){
        return;
    }
    if (headerptr->free){
        return;
    }
    headerptr->free = true;
    header* new  = (header*)((uintptr_t)headerptr + sizeof(header) + headerptr->size);

    // Search for the next free block and merges it
    while (new->free && (uintptr_t)new+sizeof(header) < heap_end){

        // Checks if is a valid block, if its valid, contains an copy
        // of MAGIC and exit the if block, if not break the loop, 
        // returning the function
        if (new->magic != MAGIC){
            break;
        }
        headerptr->size += new->size + sizeof(header);
        headerptr->next = new->next;
        new = (header*)((uintptr_t)headerptr + sizeof(header) + headerptr->size);
    }
}