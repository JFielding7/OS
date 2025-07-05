#include <stdio.h>

#include <kernel/gdt.h>
#include <kernel/tty.h>

void kernel_main(void) {
	gdt_init();
	terminal_init();
	printf("Barrel OS kernel says: DK won in %dns again\n", 1);
}
