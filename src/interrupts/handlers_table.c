#include <stdint.h>
#include <stddef.h>
#include "interrupts/handlers_table.h"
#include "interrupts/handler_functions.h"
#include "interrupts/common_handler.h"

void (*handlers[256])(registers_t*) = {
    [0] = divide_by_zero, //ISR 0
    
    [1] = debug_handler, //ISR 1
    
    [2] = non_maskable_interrupt_handler, //ISR 2
    
    [3] = breakpoint_handler, //ISR 3
    
    [4] = overflow_handler, //ISR 4
    
    [5] = bound_exception_handler, //ISR 5
    
    [6] = invalid_opcode_handler, //ISR 6
    
    [7] = generic_handler, //ISR 7
    
    [8] = double_fault_handler, //ISR 8
    
    [9] = generic_handler, //ISR 9
    
    [10] = generic_handler, //ISR 10
    
    [11] = generic_handler, //ISR 11
    
    [12] = generic_handler, //ISR 12
    
    [13] = general_fault_exception_handler, //ISR 13
    
    [14] = page_fault_handler, //ISR 14
    
    [15] = generic_handler, //ISR 15
    
    [16] = generic_handler, //ISR 16
    
    [17] = generic_handler, //ISR 17
    
    [18] = generic_handler, //ISR 18
    
    [19] = generic_handler, //ISR 19
    
    [20] = generic_handler, //ISR 20
    
    [21] = generic_handler, //ISR 21
    
    [22] = generic_handler, //ISR 22
    
    [23] = generic_handler, //ISR 23
    
    [24] = generic_handler, //ISR 24
    
    [25] = generic_handler, //ISR 25
    
    [26] = generic_handler, //ISR 26
    
    [27] = generic_handler, //ISR 27
    
    [28] = generic_handler, //ISR 28
    
    [29] = generic_handler, //ISR 29
    
    [30] = generic_handler, //ISR 30
    
    [31] = generic_handler, //ISR 31

    [32] = pit_timer_handler, //IRQ 0
    
    [33] = keyboard_handler //IRQ 1
    
};
