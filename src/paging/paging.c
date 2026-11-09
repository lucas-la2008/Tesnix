#include "paging/paging.h"
#include "multiboot/multiboot.h"
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
    
    for (int i = 0; i < 1024; i++){
        page_directory[i] = 0x00000000;
    }

    for (size_t i = 0; i < 1024; i++){
        first_page_table[i]  = (i * 0x1000) | 3; // Cobre de 0 a 4 MB
        second_page_table[i] = ((i + 1024) * 0x1000) | 3; // Cobre de 4 a 8 MB
        third_page_table[i]  = ((i + 2048) * 0x1000) | 3; // Cobre de 8 a 12 MB
        fourth_page_table[i] = ((i + 3072) * 0x1000) | 3; // Cobre de 12 a 16 MB
    }

    page_directory[0] = ((uintptr_t)first_page_table) | 3;
    page_directory[1] = ((uintptr_t)second_page_table) | 3;
    page_directory[2] = ((uintptr_t)third_page_table) | 3;
    page_directory[3] = ((uintptr_t)fourth_page_table) | 3;
    load_page_directory(page_directory);
    enable_paging();
}