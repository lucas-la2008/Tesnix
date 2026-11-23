#ifndef __PHYSICAL_MEMORY_MANAGEMENT__
#define __PHYSICAL_MEMORY_MANAGEMENT__

#include "multiboot/multiboot.h"
#include <lib/kstdint.h>
#include <lib/kstddef.h>

typedef struct{
    uintptr_t phys_addr;
    uint32_t* blocks;
    size_t num_blocks;
} bitmapSections;  

typedef struct{
    bitmapSections* ptr;
    size_t size;
} bitmapPages;

void init_pmm(multiboot_info_t* info);
uintptr_t alloc_phys_addrs();

#endif