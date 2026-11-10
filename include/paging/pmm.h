#ifndef __PHYSICAL_MEMORY_MANAGEMENT__
#define __PHYSICAL_MEMORY_MANAGEMENT__

#include <lib/kstdint.h>
#include "multiboot/multiboot.h"

uintptr_t init_pmm(multiboot_info_t* info);
uintptr_t alloc_phys_addrs();

#endif