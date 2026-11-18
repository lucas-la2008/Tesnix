#include "paging/pmm.h"
#include "multiboot/multiboot.h"
#include <lib/kstdbool.h>
#include <lib/kstdint.h>
#include <lib/kstddef.h>

// Warning! read only variable, must not be modified
extern uintptr_t heap_end;

extern char kernel_start;

bitmapPages bitmap = {NULL,0};

void init_pmm(multiboot_info_t* info){

    bitmap.ptr = (uint32_t*)heap_end;
    
    uintptr_t final_pos = info->mmap_addr + info->mmap_length;

    uintptr_t total_ram = 0;
    multiboot_memory_map_t* entry = NULL;
    for(uintptr_t current_pos = info->mmap_addr;current_pos < final_pos; current_pos += entry->size + sizeof(entry->size)){
        entry = (multiboot_memory_map_t*)current_pos;

        if (entry->addr+entry->len > total_ram){
            if (entry->type <= 4){
                total_ram = entry->addr+entry->len;
            }
        }
    }

    bitmap.max_addr = (((total_ram + 32767) >> 15) + 4095) & ~4095;

    for (size_t i= 0; i < bitmap.max_addr; i++){
        bitmap.ptr[i] = 0xFFFFFFFF;
    }

    entry = NULL;
    for(uintptr_t current_pos = info->mmap_addr;current_pos < final_pos; current_pos += entry->size + sizeof(entry->size)){
        entry = (multiboot_memory_map_t*)current_pos;
        if (entry->type != 1){
            continue;
        }
        size_t addr = entry->addr;
        size_t final_addr = addr+ entry->len;
        
        size_t aligned_addr = (addr + 4095) & ~4095;
        size_t aligned_final_addr = final_addr & ~4095;
        
        size_t count = (aligned_final_addr - aligned_addr) >> 12;
        /*
        * Count is the amount of secure pages in this memory section
        * The bytes index in the bitmap is the page initial address divided by 4096*8(or 32768)
        * The bit in the found byte is found doing % 8 or & 7 in the byte index
        */
        for (size_t i= 0; i< count; i++){
            size_t actual_page = aligned_addr + (i*4096);
            size_t page_index = actual_page >> 12; // Same as dividing by 4096
            size_t byte_index = page_index >> 5; // Same as dividing by 8
            size_t bit_pos = page_index & 31; // Get the bit index by removing everything beyond bit 2
            bitmap.ptr[byte_index] |= ~(1 << bit_pos);
        }   
    }
    uintptr_t kernel_mem_start = (uintptr_t)&kernel_start;
    uintptr_t kernel_mem_end = ((uintptr_t)bitmap.ptr + bitmap.max_addr);
    uintptr_t kernel_mem_size = kernel_mem_end - kernel_mem_start;
    size_t count = kernel_mem_size >> 12;
    for (size_t i = 0; i < count; i++){
        size_t page_addr = kernel_mem_start + (i*4096);
        size_t page_index = page_addr >> 12;
        size_t byte_index = page_index >> 5;
        size_t bit_pos = page_index & 31;
        bitmap.ptr[byte_index] |= 1<<bit_pos;
    }
}

uintptr_t alloc_phys_addrs(){
    bool found = false;
    size_t page_group = 0;
    while(!found){
        if (bitmap.ptr[page_group] == 0xFF){
            continue;
        }
        for (size_t j = 0; j < 32; j++){
            if ((bitmap.ptr[page_group] >> j) & 1){
                
            }
        }
        page_group++;
    }
}