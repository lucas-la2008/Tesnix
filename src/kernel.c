#include "descriptors/gdt.h"
#include "descriptors/idt.h"
#include "multiboot/multiboot.h"
#include "pic/pic.h"
#include "paging/paging.h"

#include "terminal/terminal.h"
#include "memalloc/kmalloc.h"

void kernel_main(multiboot_info_t* multiboot_info)
{
    __asm__ volatile("cli");
    
    heap_init();
    gdt_init();
    idt_init();
    PIC_remap(0x20,0x28);
    terminal_initialize();
    set_paging(multiboot_info);
    
    __asm__ volatile("sti");
    for (;;)
    {
        __asm__ volatile("hlt");
    }
}