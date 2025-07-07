#include <stddef.h>
#include <stdint.h>

#include "../include/kernel/gdt.h"


struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct gdt_entry gdt[GDT_SIZE];


extern void load_gdt(uint16_t limit, uint32_t offset);

void gdt_set_entry(size_t i, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    struct gdt_entry *entry = gdt + i;
    entry->limit_low    = limit & 0xFFFF;
    entry->base_low     = base & 0xFFFF;
    entry->base_middle  = (base >> 16) & 0xFF;
    entry->base_high    = (base >> 24) & 0xFF;
    entry->access       = access;
    entry->granularity  = ((limit >> 16) & 0x0F) | (flags << 4);
}

void gdt_init() {
    gdt_set_entry(GDT_NULL_INDEX, 0, 0, 0, 0);
    gdt_set_entry(GDT_KERNEL_CODE_INDEX, 0, 0xFFFFF, GDT_KERNEL_CODE_ACCESS, KERNEL_CODE_FLAGS);
    gdt_set_entry(GDT_KERNEL_DATA_INDEX, 0, 0xFFFFF, GDT_KERNEL_DATA_ACCESS, KERNEL_DATA_FLAGS);

    load_gdt(sizeof(gdt) - 1, (uint32_t) gdt);
}
