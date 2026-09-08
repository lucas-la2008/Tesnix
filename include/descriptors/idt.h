#ifndef __IDT__
#define __IDT__

#include <stdint.h>
#include <stddef.h>

void idt_init(void);
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
extern void idt_load(void *idtr);
void idt_install_irq(void);

#endif