#ifndef __HANDLER_FUNCTIONS__
#define __HANDLER_FUNCTIONS__

#include "interrupts/common_handler.h"
#include <stdint.h>

//ISR 0
void divide_by_zero(registers_t *r);

// ISR 1
void debug_handler(registers_t *r);

//ISR 2
void non_maskable_interrupt_handler(registers_t *r);

//ISR 3
void breakpoint_handler(registers_t *r);

//ISR 4
void overflow_handler(registers_t *r);

//ISR 5
void bound_exception_handler(registers_t *r);

//ISR 6
void invalid_opcode_handler(registers_t *r);

//ISR 8
void double_fault_handler(registers_t *r);

//ISR 13
void general_fault_exception_handler(registers_t *r);

//ISR 14
void page_fault_handler(registers_t *r);

//IRQ 0
extern uint64_t timer;
void pit_timer_handler(registers_t *r);

//IRQ 1
void keyboard_handler(registers_t *r);

void generic_handler(registers_t *r);

#endif