#ifndef __GDT_H__
#define __GDT_H__

#include <lib/kstdint.h>
#include <lib/kstddef.h>

void gdt_init(void);
void gdt_set_entry(size_t index, uint32_t limit, uint32_t base, uint8_t acess, uint8_t flags);
extern void gdt_load(void* gdtr);

#endif