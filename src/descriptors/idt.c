#include <stdint.h>
#include <stddef.h>
#include "descriptors/idt.h"

typedef struct idt_ptr {
    uint16_t limit;
    uint32_t base;
}__attribute__((packed)) idt_ptr_t;

typedef struct idt_entry {
    uint16_t isr_low;
    uint16_t selector;
    uint8_t reserved;
    uint8_t attributes;
    uint16_t isr_high;
}__attribute__((packed)) idt_entry_t;

__attribute__((aligned(0x10)))
static idt_entry_t idt[256];

static idt_ptr_t idtr;

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags){
    idt_entry_t *descriptor = &idt[vector];

    descriptor->isr_low = (uint32_t)isr & 0xffff;
    descriptor->selector = 0x08;
    descriptor->attributes = flags;
    descriptor->isr_high = (uint32_t)isr>>16;
    descriptor->reserved = 0;
}

extern void* isr_stub_table[];
extern void* irq_stub_table[];

void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)(sizeof(idt) - 1);

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    }

    for (uint8_t irq = 0; irq < 16; irq++)
    {
        idt_set_descriptor(irq+32, irq_stub_table[irq], 0x8E);
    }

    idt_load(&idtr);
}
