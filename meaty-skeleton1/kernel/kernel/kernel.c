#include <stdio.h>
#include <limits.h>
#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();
	printf("%u\n", 1 << 31);
}
