#include <stdint.h>
#include <stddef.h>
#include "interrupts/handler_functions.h"
#include "interrupts/common_handler.h"
#include "terminal/terminal.h"
#include "terminal/keyboard.h"
#include "io.h"
#include "functions.h"

//ISR 0
void divide_by_zero(registers_t* r)
{
    (void)r;
    const char characteres[] ="ERROR,Division by zero!";
    size_t count = 0;
    clear(false);
    while (characteres[count])
    {
        vga_putchar(characteres[count]);
        count++;
    }
    while (1) {
        __asm__ volatile("cli");
        __asm__ volatile("hlt");
    }
}

// ISR 1
void debug_handler(registers_t* r)
{
    (void)r;
    vga_writestring("Debug");
    return;
}

//ISR 2
void non_maskable_interrupt_handler(registers_t *r)
{
    (void)r;
    clear(false);
    const char characters[] = "\nHardware failure detected!";
    size_t count = 0;
    while (characters[count])
    {
        vga_putchar(characters[count++]);
    }

    while (1)
    {
        __asm__ volatile("cli");
        __asm__ volatile("hlt");
    }
}

//ISR 3
void breakpoint_handler(registers_t *r)
{
    char info[5][11];
    toStr(r->eip, info[0]);
    toStr(r->esp, info[1]);
    toStr(r->int_no, info[2]);
    
    vga_writestring("EIP:");
    vga_writestring(info[0]);

    __asm__ volatile("hlt");
}

//ISR 4
void overflow_handler(registers_t *r)
{
    char info[15];
    toStr(r->eip, info);
    vga_writestring(info);

    for(;;)
    {
        __asm__ volatile("cli");
        __asm__ volatile("hlt");
    }
}

//ISR 5
void bound_exception_handler(registers_t *r)
{
    (void)r;
    vga_writestring("BOUND Exception!");
    for(;;)
    {
        __asm__ volatile("cli");
        __asm__ volatile("hlt");
    }
}

//ISR 6
void invalid_opcode_handler(registers_t *r)
{
    char info[2][16];
    toHex(r->eip, info[0]);
    toHex(r->int_no, info[1]);
    vga_writestring("INVALID OPCODE!");
    vga_writestring("PANIC!");
    vga_writestring("EIP: ");
    vga_writestring(info[0]);
    vga_writestring("INT_NO: ");
    vga_writestring(info[1]);
    
    while (1)
    {
        __asm__ volatile("cli");
        __asm__ volatile("hlt");
    }
}

//ISR 8
void double_fault_handler(registers_t *r)
{
    char error[10];
    toStr(r->err_code, error);
    vga_writestring("Kernel panic!");
    vga_writestring("DOUBLE FAULT!\nError code: ");
    vga_writestring(error);
    vga_writestring("\n");

    for (;;)
    {
        __asm__ volatile("cli");
        __asm__ volatile("hlt");
    }
}

//ISR 13
void general_fault_exception_handler(registers_t *r)
{
    char info[10][20];
    toHex(r->eip, info[0]);
    toHex(r->eax, info[1]);
    toHex(r->err_code, info[2]);

    vga_writestring("GENERAL FAULT PROTECTION!");
    vga_writestring("EIP: ");
    vga_writestring(info[0]);
    vga_writestring("EAX: ");
    vga_writestring(info[1]);
    vga_writestring("ERROR CODE: ");
    vga_writestring(info[2]);

    for(;;)
    {
        __asm__ volatile("cli");
        __asm__ volatile("hlt");
    }
}

//ISR 14
void page_fault_handler(registers_t *r)
{
    size_t address;
    __asm__ volatile("mov %%cr2, %0": "=r"(address));
    
    char info[5][10];
    toHex(r->eip, info[0]);
    toHex(address, info[1]);
    toHex(r->err_code, info[2]);

    vga_writestring("PAGE FAULT!");

    vga_writestring("EIP: ");
    vga_writestring(info[0]);
    vga_writestring("CR2: ");
    vga_writestring(info[1]);
    vga_writestring("ERROR CODE:");
    vga_writestring(info[2]);

    for(;;)
    {
        __asm__ volatile("cli");
        __asm__ volatile("hlt");
    }
}

uint64_t timer = 0;
//IRQ 0
void pit_timer_handler(registers_t *r)
{
    (void)r;
    timer++;
}

//IRQ 1
void keyboard_handler(registers_t* r)
{
    (void)r;
    if (inb(0x64) & 0x01)
    {
        uint8_t code = inb(0x60);
        char str = keyboard_char(code);
        if (str == '\0')
        {
            return;
        }
        vga_putchar(str);
    }
    return;
}

void generic_handler(registers_t* r)
{
    (void)r;
    const char characteres[] ="generic message!";
    size_t count = 0;
    while (characteres[count])
    {
        vga_putchar(characteres[count]);
        count++;
    }
    return;
}
