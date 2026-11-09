#include "paging/paging.h"
#include <lib/kstddef.h>

extern uintptr_t heap_start;
extern uintptr_t heap_end;
extern int64_t MAGIC;

_Alignas(4096) uint32_t page_directory[1024];

_Alignas(4096) uint32_t first_page_table [1024];
_Alignas(4096) uint32_t second_page_table[1024];
_Alignas(4096) uint32_t third_page_table[1024];
_Alignas(4096) uint32_t fourth_page_table[1024];

void set_paging(multiboot_info_t* header){
    
    for (int i = 0; i < 1024; i++){
        page_directory[i] = 0x00000002;
    }

    for (size_t i = 0; i < 1024; i++){
        first_page_table[i] = (i*0x1000) | 3;
    }

    page_directory[0] = ((size_t)first_page_table) | 3;
}
