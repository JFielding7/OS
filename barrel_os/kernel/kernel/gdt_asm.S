    .section .data
gdtr:
    .word 0 # limit
    .long 0 # base

    .section .text
    .global load_gdt
load_gdt:
    movw 4(%esp), %ax
    movw %ax, gdtr
    movl 8(%esp), %eax
    movl %eax, gdtr+2
    lgdt gdtr
    ret
