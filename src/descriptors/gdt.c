#include <stdint.h>
#include <stddef.h>
#include "descriptors/gdt.h"

typedef struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct gdt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

gdt_ptr_t gdtr;

__attribute__((aligned(0x10)))
gdt_entry_t gdt[3];

void gdt_set_entry(size_t index, uint32_t limit, uint32_t base, uint8_t access, uint8_t flags)
{
    gdt[index].base_low = base & 0xffff;
    gdt[index].base_middle = (base >> 16) & 0xff;
    gdt[index].base_high = (base >> 24) & 0xff;
    gdt[index].limit_low = limit & 0xffff;
    gdt[index].granularity = (limit >> 16) & 0x0f;
    gdt[index].granularity |= (flags << 4);
    gdt[index].access = access;
}

void gdt_init(void)
{
    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = (uint32_t)&gdt;

    gdt_set_entry(0,0,0,0,0);
    gdt_set_entry(1,0x000fffff,0x0,0x9a,0x0c);
    gdt_set_entry(2,0x000fffff,0x0,0x92,0x0c);

    gdt_load(&gdtr);
}