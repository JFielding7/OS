#include <string.h>

int strcmp(const char *s0, const char *s1) {
    while (*s0 && *s1 && *s0 == *s1) {
        s0++;
        s1++;
    }

    return *s0 - *s1;
}