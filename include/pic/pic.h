#ifndef __8259_PIC__
#define __8259_PIC__
#include <stdint.h>

void PIC_sendEOI(uint32_t irq);
void PIC_remap(uint32_t offset1, uint32_t offset2);
uint16_t __pic_get_irq_reg(int ocw3);
uint16_t pic_get_irr(void);
uint16_t pic_get_isr(void);

#endif