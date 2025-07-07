#include <stdio.h>

#include "../include/kernel/tty.h"
#include "../include/kernel/gdt.h"
#include "../include/kernel/idt.h"

void kernel_main(void) {
	gdt_init();
	idt_init();
	terminal_init();

	printf("%d\n", IDT_DEFAULT_TYPE_ATTRIBUTES);

	printf("Barrel OS kernel says: DK won in %dns again\n", 1);
	__asm__ volatile ("int $0x80");
	__asm__ volatile ("int $0x81");
}
