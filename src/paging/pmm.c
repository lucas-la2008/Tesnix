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

    bitmap.size = (((total_ram + 131072) >> 17) + 4095) & ~4095LLU;

    for (size_t i= 0; i < bitmap.size; i++){
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
        
        // Count is the amount of secure pages in this memory section
        size_t count = (aligned_final_addr - aligned_addr) >> 12;

        /*
        * The bytes index in the bitmap is the page initial address divided by 4096*8(or 32768)
        * The bit in the found byte is found doing % 8 or & 7 in the byte index
        */
        for (size_t i= 0; i< count; i++){
            size_t actual_page = aligned_addr + (i*4096);
            size_t page_index = actual_page >> 12; // Same as dividing by 4096
            size_t byte_index = page_index >> 5; // Same as dividing by 32
            size_t bit_pos = page_index & 31; // Get the bit index by removing everything beyond bit 2
            bitmap.ptr[byte_index] |= ~(1 << bit_pos);
        }   
    }
<<<<<<< HEAD
    uintptr_t kernel_mem_start = (uintptr_t)&kernel_start;
    uintptr_t kernel_mem_end = ((uintptr_t)bitmap.ptr + (bitmap.size*sizeof(uint32_t)));
    uintptr_t kernel_mem_size = kernel_mem_end - kernel_mem_start;
    size_t count = kernel_mem_size >> 12;
    for (size_t i = 0; i < count; i++){
        size_t page_addr = kernel_mem_start + (i*4096);
        size_t page_index = page_addr >> 12;
        size_t group_index = page_index >> 5;
        size_t bit_pos = page_index & 31;
        bitmap.ptr[group_index] |= 1U<<bit_pos;
    }
}

uintptr_t alloc_phys_addrs(){
    for(size_t page_group = 0; page_group < bitmap.size; page_group++){
        if (bitmap.ptr[page_group] == 0xFFFFFFFF ){
            continue;
        }
        for (size_t j = 0; j < 32; j++){
            if (((bitmap.ptr[page_group] >> j) & 1) == 0){
                bitmap.ptr[page_group] |= 1<< j;
                return ((page_group * 32 + j) * 4096);
            }
=======

    sections.size = count;
    uintptr_t previous_num_blocks = 0;

    uintptr_t base_blocks_addr = (uintptr_t)sections.ptr + sections.size * sizeof(bitmapSections);
    base_blocks_addr = (base_blocks_addr + ((uintptr_t)sizeof(uint32_t) - 1u)) & ~((uintptr_t)sizeof(uint32_t) - 1u);

    for (size_t i = 0; i < sections.size; i++){
        sections.ptr[i].blocks = (uint32_t*)(base_blocks_addr + previous_num_blocks * sizeof(uint32_t));
        previous_num_blocks += sections.ptr[i].num_blocks;
    }
    
    for (size_t i= 0; i < sections.size; i++){
        for (size_t j = 0; j < sections.ptr[i].num_blocks; j++){
            sections.ptr[i].blocks[j] = 0;
        }
    }

    
    uintptr_t addr = (uintptr_t)&kernel_start;
    uintptr_t final_addr = (uintptr_t)sections.ptr[sections.size - 1].blocks + (sections.ptr[sections.size-1].num_blocks * sizeof(uint32_t));
    
    uintptr_t aligned_addr = addr & ~4095u;
    uintptr_t aligned_final_addr = (final_addr + 4095u) & ~4095u;
    
    // page_count is the amount of secure pages in this memory section
    size_t page_count = (aligned_final_addr - aligned_addr) >> 12;

    /*
    *   The byte index in the bitmap is the page initial address divided by 4096*32(or 131072)
    * and verifying in which section it belongs to.
    *   The bit in the found byte is found doing % 32 or & 31 in the byte index.
    */
    bitmapSections* sec = NULL;
    uintptr_t section_start = 0;
    uintptr_t section_end = 0;
    
    for (size_t i= 0; i< page_count; i++){
        uintptr_t actual_page = aligned_addr + ((uintptr_t)i*4096u);

        if (actual_page < section_start || actual_page >= section_end || sec == NULL){
            size_t section_idx = find_section(actual_page);
            if (section_idx >= sections.size){            
                sec = NULL;
                section_start = 0;
                section_end = 0;
                continue;
            }
            
            sec = &sections.ptr[section_idx];
            section_start = sec->phys_addr;
            section_end = sec->phys_addr + (sec->num_blocks << 17); // Same as multiplying by 4096*32
        }

        size_t page_index = (actual_page - section_start) >> 12;
        size_t word_index = page_index >> 5; // Same as dividing by 32
        size_t bit_pos = page_index & 31u; // Get the bit index by removing everything beyond bit 4
        sec->blocks[word_index] |= (1u << bit_pos);
    }   
}

uintptr_t alloc_phys_addrs(){
    for(size_t section = 0; section < sections.size; section++){
        bitmapSections sec = sections.ptr[section];
        for (size_t block = 0; block < sec.num_blocks; block++){
            
>>>>>>> aee23fd (feat(pmm): finish init_pmm function)
        }
    }
    return 0;
}