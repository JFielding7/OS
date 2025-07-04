#include <stddef.h>
#include <stdint.h>

#if defined(__linux__)
#error "Did not use cross-compiler"
#endif

#if !defined(__i386__)
#error "Did not use ix86-elf compiler"
#endif

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000

typedef struct {
	size_t row;
	size_t col;
	uint8_t color;
	uint16_t *buffer;
} terminal_t;

terminal_t terminal;

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GRAY = 7,
	VGA_COLOR_DARK_GRAY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str) {
	size_t len = 0;

	while (str[len]) {
		len++;
	}

	return len;
}

static inline size_t terminal_buffer_index(size_t row, size_t col) {
	return row * VGA_WIDTH + col;
}

static void inline terminal_set_entry(size_t row, size_t col, unsigned char c) {
	terminal.buffer[terminal_buffer_index(row, col)] = vga_entry(c, terminal.color);
}

void terminal_initialize(void) {
	terminal.row = 0;
	terminal.col = 0;
	terminal.color = vga_entry_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK);
	terminal.buffer = (uint16_t*) VGA_MEMORY;

	for (size_t row = 0; row < VGA_HEIGHT; row++) {
		for (size_t col = 0; col < VGA_WIDTH; col++) {
			terminal_set_entry(row, col, ' ');
		}
	}
}

void terminal_scroll_down() {
	for (size_t row = 0; row < VGA_HEIGHT - 1; row++) {
		for (size_t col = 0; col < VGA_WIDTH; col++) {
			size_t index = terminal_buffer_index(row, col);
			terminal.buffer[index] = terminal.buffer[index + VGA_WIDTH];
		}
	}

	for (size_t col = 0; col < VGA_WIDTH; col++) {
		terminal_set_entry(VGA_HEIGHT - 1, col, ' ');
	}
}

void terminal_putchar(char c)
{
	if (c == '\0') {
		return;
	}

	if (c == '\n') {
		terminal.col = 0;

		if (terminal.row < VGA_HEIGHT - 1) {
			terminal.row++;
		} else {
			terminal_scroll_down();
		}
	} else {
		if (terminal.row == VGA_HEIGHT) {
			terminal_scroll_down();
			terminal.row--;
		}

		terminal_set_entry(terminal.row, terminal.col, c);
		if (++terminal.col == VGA_WIDTH) {
			terminal.col = 0;
			terminal.row++;
		}
	}
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++) {
		terminal_putchar(data[i]);
	}
}

void terminal_write_str(const char* data) {
	terminal_write(data, strlen(data));
}

void kernel_main(void) {
	terminal_initialize();
	terminal_write_str("DK won\nDK won again\nDK won\nDK won\nDK won\n"
		"DK won\nDK won\nDK won\nDK won\nDK won\n"
		"DK won\nDK won\nDK won\nDK won\nDK won\n"
		"DK won\nDK won\nDK won\nDK won\nDK won\n"
		"DK won\nDK won\nDK won\nDK won\nDK wins\n");
}
