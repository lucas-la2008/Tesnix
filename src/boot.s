.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    mov $stack_top, %esp
    cmpl $0x2BADB002, %eax
    jne boot_error
    pushl %ebx
    call kernel_main
    cli

hang: hlt
    jmp hang

boot_error:
    cli
    hlt
    jmp boot_error

.size _start, . - _start
