    .section .data
idtr:
    .word 0 # limit
    .long 0 # offset

    .section .text
    .global load_idt
load_idt:
    movw 4(%esp), %ax
    movw %ax, idtr
    movl 8(%esp), %eax
    movl %eax, idtr+2
    lidt idtr
    ret
