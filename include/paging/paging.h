#ifndef __PAGING__
#define __PAGING__

#include "multiboot/multiboot.h"
#include <lib/kstdint.h>



void set_paging(multiboot_info_t*);
extern void load_page_directory(uint32_t* dir);
extern void enable_paging(void);

#endif