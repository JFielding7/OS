#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_col;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

static inline size_t terminal_buffer_index(size_t row, size_t col) {
	return row * VGA_WIDTH + col;
}

static inline void terminal_set_entry(size_t row, size_t col, unsigned char c) {
	terminal_buffer[terminal_buffer_index(row, col)] = vga_entry(c, terminal_color);
}

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_col = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) VGA_MEMORY;

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
			terminal_buffer[index] = terminal_buffer[index + VGA_WIDTH];
		}
	}

	for (size_t col = 0; col < VGA_WIDTH; col++) {
		terminal_set_entry(VGA_HEIGHT - 1, col, ' ');
	}
}

void terminal_put_new_line() {
	terminal_col = 0;
	if (terminal_row < VGA_HEIGHT - 1) {
		terminal_row++;
	} else {
		terminal_scroll_down();
	}
}

void terminal_put_char_default(char c) {
	if (terminal_row == VGA_HEIGHT) {
		terminal_scroll_down();
		terminal_row--;
	}

	terminal_set_entry(terminal_row, terminal_col, c);
	if (++terminal_col == VGA_WIDTH) {
		terminal_col = 0;
		terminal_row++;
	}
}

void terminal_putchar(char c)
{
	switch (c) {
		case '\0':
			break;
		case '\n':
			terminal_put_new_line();
			break;
		default:
			terminal_put_char_default(c);
			break;
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
