#ifndef __PAGING__
#define __PAGING__

#include <lib/kstdint.h>
_Alignas(1024) uint32_t page_directory[1024];
_Alignas(1024) uint32_t first_page_table[1024];
void set_paging(void);
#endif