#ifndef __IDT_H__
#define __IDT_H__

#include <lib/kstdint.h>
#include <lib/kstddef.h>

void idt_init(void);
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
extern void idt_load(void *idtr);
void idt_install_irq(void);

#endif