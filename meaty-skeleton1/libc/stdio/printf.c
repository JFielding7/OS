#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

static size_t uint_base10(unsigned int val, char *buffer) {
	if (val == 0) {
		buffer[0] = '0';
		return 1;
	}

	size_t bytes_written = 0;
	int pow10 = 1;

	while (val) {
		unsigned int digit = val % 10;
		buffer[bytes_written++] = (char) digit + '0';

		val /= 10;
		pow10 *= 10;
	}

	for (size_t i = 0; i < (bytes_written >> 1); i++) {
		char temp = buffer[i];
		buffer[i] = buffer[bytes_written - i - 1];
		buffer[bytes_written - i - 1] = temp;
	}

	return bytes_written;
}

static size_t int_base10(int val, char *buffer) {
	if (val < 0) {
		buffer[0] = '-';
		return 1 + uint_base10(-val, buffer + 1);
	}

	return uint_base10(val, buffer);
}

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			char c = (char) va_arg(parameters, int);
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 'd') {
			format++;
			char buffer[sizeof(int) << 3];
			int val = va_arg(parameters, int);
			size_t bytes = int_base10(val, buffer);
			if (maxrem < bytes) {
				return -1;
			}
			if (!print(buffer, bytes)) {
				return -1;
			}
			written += bytes;
		} else if (*format == 'u') {
			format++;
			char buffer[sizeof(unsigned int) << 3];
			unsigned int val = va_arg(parameters, unsigned int);
			size_t bytes = uint_base10(val, buffer);
			if (maxrem < bytes) {
				return -1;
			}
			if (!print(buffer, bytes)) {
				return -1;
			}
			written += bytes;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
