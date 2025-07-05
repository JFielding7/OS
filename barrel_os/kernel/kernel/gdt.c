#include <stddef.h>
#include <stdint.h>

#include <kernel/gdt.h>

#define NUM_GDT_ENTRIES 3

#define PRESENT (1 << 7)
#define HIGH_PRIVLEDGE  0
#define LOW_PRIVLEDGE  (3 << 5)
#define SYSTEM_SEG  0
#define CODE_OR_DATA_SEG  (1 << 4)
#define EX (1 << 3)
#define DIR_UP  0
#define DIR_DOWN  (1 << 2)
#define RD   (1 << 1)
#define RDWR (1 << 1)
#define ACCESSED  1

#define KERNEL_CODE_ACCESS (PRESENT | HIGH_PRIVLEDGE | CODE_OR_DATA_SEG | EX | DIR_UP | RD)
#define KERNEL_DATA_ACCESS (PRESENT | HIGH_PRIVLEDGE | CODE_OR_DATA_SEG | DIR_UP | RDWR)

#define FLAG_4K_GRAN  (1 << 3)
#define FLAG_32BIT    (1 << 2)

#define KERNEL_CODE_FLAGS (FLAG_4K_GRAN | FLAG_32BIT)
#define KERNEL_DATA_FLAGS (FLAG_4K_GRAN | FLAG_32BIT)

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


struct gdt_entry gdt[NUM_GDT_ENTRIES];


extern void load_gdt(uint16_t limit, uint32_t base);

void gdt_create_entry(size_t i, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    struct gdt_entry *entry = gdt + i;
    entry->limit_low    = limit & 0xFFFF;
    entry->base_low     = base & 0xFFFF;
    entry->base_middle  = (base >> 16) & 0xFF;
    entry->base_high    = (base >> 24) & 0xFF;
    entry->access       = access;
    entry->granularity  = ((limit >> 16) & 0x0F) | (flags << 4);
}

void gdt_init() {
    gdt_create_entry(0, 0, 0, 0, 0);
    gdt_create_entry(1, 0, 0xFFFFF, KERNEL_CODE_ACCESS, KERNEL_CODE_FLAGS);
    gdt_create_entry(2, 0, 0xFFFFF, KERNEL_DATA_ACCESS, KERNEL_DATA_FLAGS);

    load_gdt(sizeof(gdt) - 1, (uint32_t) &gdt);
}
