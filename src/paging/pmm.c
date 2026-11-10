#include "paging/pmm.h"
#include "multiboot/multiboot.h"
#include <lib/kstdint.h>
#include <lib/kstddef.h>

// Warning! read only variable, must not be modified
extern uintptr_t heap_end;

uint8_t* bitmap_ptr = NULL;
size_t bitmap_length = 0;

uintptr_t init_pmm(multiboot_info_t* info){

    bitmap_ptr = (uint8_t*)heap_end;
    
    uintptr_t current_pos = info->mmap_addr;
    uintptr_t final_pos = info->mmap_addr + info->mmap_length;

    uintptr_t total_ram = 0;
    
    while(current_pos < final_pos){
        multiboot_memory_map_t* entry = (multiboot_memory_map_t*)current_pos;

        if (entry->addr+entry->len > total_ram){
            if (entry->type <= 4){
                total_ram = entry->addr+entry->len;
            }
        }
        
        current_pos+= entry->size + sizeof(entry->size);
    }

    bitmap_length = (total_ram + 32767) >> 15;

    for (size_t i= 0; i < bitmap_length; i++){
        bitmap_ptr[i] = 0xFF;
    }

    current_pos = info->mmap_addr;

    while (current_pos < final_pos){
        multiboot_memory_map_t* entry = (multiboot_memory_map_t*)current_pos;

        
        
        current_pos += entry->size + sizeof(entry->size);
    }
}

uintptr_t alloc_phys_addrs(){
}