#include "paging/pmm.h"
#include "multiboot/multiboot.h"
#include <lib/kstdbool.h>
#include <lib/kstdint.h>
#include <lib/kstddef.h>

// Warning! read only variable, must not be modified
extern uintptr_t heap_end;

extern char kernel_start;

bitmapPages sections = {NULL,0};

size_t find_section(uintptr_t);

void init_pmm(multiboot_info_t* info){

    sections.ptr = (bitmapSections*)heap_end;
    
    uintptr_t final_pos = info->mmap_addr + info->mmap_length;

    uintptr_t count = 0;
    multiboot_memory_map_t* entry = NULL;
    for(uintptr_t current_pos = info->mmap_addr;
        current_pos < final_pos;
        current_pos += entry->size + sizeof(entry->size))
    {
        entry = (multiboot_memory_map_t*)current_pos;

        if (entry->type != 1) {
            continue;
        }
        
        uint64_t start = (entry->addr + 4095ULL) & ~4095ULL;
        uint64_t end = (entry->addr + entry->len) & ~4095ULL;
        
        sections.ptr[count].phys_addr = start;
        sections.ptr[count].num_blocks = (((end - start) >> 12ULL) + 31ULL) >> 5ULL;
        count++;
    }

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
            if (section_idx >= sections.size || section_idx == 0){            
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
        bitmapSections* sec = &sections.ptr[section];
        for (size_t block = 0; block < sec->num_blocks; block++){
            if (sec->blocks[block] == 0xFFFFFFFF){
                continue;
            }
            size_t bit_index = __builtin_ctzl(~sec->blocks[block]);
            if (bit_index == 0){
                bit_index = __builtin_clzl(~sec->blocks[block]);
            }
            sec->blocks[block] |= 1u << bit_index;
            return + sec->phys_addr + (sec->blocks[block] << 17);
        }
    }
    return 0;
}

size_t find_section(uintptr_t address){
    for (size_t i = 0; i < sections.size; i++){
        bitmapSections section = sections.ptr[i];
        uintptr_t final_addr = section.phys_addr + (section.num_blocks * sizeof(uint32_t));
        if (address > section.phys_addr && address < final_addr){
            return i;
        }
    }
    return 0;
}
