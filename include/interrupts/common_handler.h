#ifndef __EXCEPTION_HANDLERS_H__
#define __EXCEPTION_HANDLERS_H__

#include <lib/kstdint.h>
#include <lib/kstddef.h>
typedef struct registers{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no;
    uint32_t err_code;
    uint32_t eip;
}registers_t;

void common_handler(registers_t* interrupt);

#endif