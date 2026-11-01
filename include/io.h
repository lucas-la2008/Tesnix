#ifndef __IO_H__
#define __IO_H__

#include <lib/kstdint.h>

static inline uint8_t inb(uint16_t port)
{
    uint8_t value;
    __asm__ __volatile__("inb %1, %0" : "=a"(value) : "Nd" (port));
    return value;
}

static inline void outb(uint16_t port,uint8_t value)
{
    __asm__ __volatile__("outb %0, %1" : :"a" (value), "Nd"(port));
}

static inline void io_wait(void)
{
    outb(0x80,0);
}

#endif