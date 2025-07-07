#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "../include/kernel/gdt.h"
#include "../include/kernel/idt.h"


struct idt_entry {
    uint16_t offset_low;
    uint16_t gdt_segment_selector;
    uint8_t  zero;
    uint8_t  type_attributes;
    uint16_t offset_high;
}  __attribute__((packed));

struct idt_entry idt[IDT_SIZE];


extern void load_idt(uint16_t limit, uint32_t offset);
extern void isr_wrapper(void);

void print_dk() {
    puts("DK won");
}

void default_isr() {
    puts("Default isr");
    __asm__ volatile("hlt\n");
}

void idt_set_entry(size_t i, uint32_t offset, uint16_t gdt_segment_selector, uint8_t type_attributes) {
    struct idt_entry *entry = idt + i;
    entry->zero = 0;
    entry->offset_low = offset & 0xFFFF;
    entry->offset_high = (offset >> 16) & 0xFFFF;
    entry->gdt_segment_selector = gdt_segment_selector;
    entry->type_attributes = type_attributes;
}

void idt_init() {
    for (size_t i = 0; i < IDT_SIZE; i++) {
        idt_set_entry(i, (uint32_t) default_isr,
            KERNEL_CODE_SEGMENT, IDT_DEFAULT_TYPE_ATTRIBUTES);
    }

    idt_set_entry(0x80, (uint32_t) isr_wrapper,
            KERNEL_CODE_SEGMENT, IDT_DEFAULT_TYPE_ATTRIBUTES);

    load_idt(sizeof(idt) - 1, (uint32_t) idt);
}
