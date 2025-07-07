    .section .data
gdtr:
    .word 0 # limit
    .long 0 # offset

    .section .text
    .global load_gdt
load_gdt:
    movw 4(%esp), %ax
    movw %ax, gdtr
    movl 8(%esp), %eax
    movl %eax, gdtr+2
    lgdt gdtr
    ljmp $0x08, $set_segment_registers

// ljmp already sets %cs to kernel code segment (0x8)
set_segment_registers:
    // set registers to kernel data segment (0x10) to be safe
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ret
