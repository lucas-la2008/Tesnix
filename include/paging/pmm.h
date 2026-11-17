#ifndef __PHYSICAL_MEMORY_MANAGEMENT__
#define __PHYSICAL_MEMORY_MANAGEMENT__

#include "multiboot/multiboot.h"
#include <lib/kstdint.h>
#include <lib/kstddef.h>

typedef struct{
    uint8_t* ptr;
    size_t length;
} bitmapPages;

void init_pmm(multiboot_info_t* info);
uintptr_t alloc_phys_addrs();

#endif