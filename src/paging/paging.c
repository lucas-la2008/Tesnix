#include "paging/paging.h"
#include "multiboot/multiboot.h"
#include "paging/pmm.h"
#include <lib/kstddef.h>
#include <lib/kstdint.h>

extern uintptr_t heap_start;
extern uintptr_t heap_end;
extern int64_t MAGIC;

_Alignas(4096) uint32_t page_directory[1024];

_Alignas(4096) uint32_t first_page_table [1024];
_Alignas(4096) uint32_t second_page_table[1024];
_Alignas(4096) uint32_t third_page_table[1024];
_Alignas(4096) uint32_t fourth_page_table[1024];

void set_paging(multiboot_info_t* header){

    init_pmm(header);
    
    for (int i = 0; i < 1024; i++){
        page_directory[i] = 0x00000000;
    }

    for (size_t i = 0; i < 1024; i++){
        first_page_table[i]  = (i * 0x1000) | 3; // Cover from 0 to 4 MiB
        second_page_table[i] = ((i + 1024) * 0x1000) | 3; // Cover from 4 to 8 MiB
        third_page_table[i]  = ((i + 2048) * 0x1000) | 3; // Cover from 8 to 12 MiB
        fourth_page_table[i] = ((i + 3072) * 0x1000) | 3; // Cover from 12 to 16 MiB
    }

    page_directory[0] = ((uintptr_t)first_page_table) | 3;
    page_directory[1] = ((uintptr_t)second_page_table) | 3;
    page_directory[2] = ((uintptr_t)third_page_table) | 3;
    page_directory[3] = ((uintptr_t)fourth_page_table) | 3;
    load_page_directory(page_directory);
    enable_paging();
}