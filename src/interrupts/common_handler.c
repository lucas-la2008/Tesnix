#include <stdint.h>
#include <stddef.h>
#include "interrupts/handlers_table.h"
#include "interrupts/common_handler.h"
#include "pic/pic.h"

void common_handler(registers_t *interrupt)
{
    size_t int_no = interrupt->int_no;
    if (handlers[int_no] != NULL)
    {
        handlers[int_no](interrupt);
    }
    if (int_no >= 32 && int_no <=47)
    {
        PIC_sendEOI(int_no-32);
    }
    return;
}
