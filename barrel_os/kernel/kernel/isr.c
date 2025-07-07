#include <stdio.h>

#include "../include/kernel/isr.h"

void print_dk() {
    puts("DK won");
}

void default_isr(int interrupt_vector) {
    printf("Default ISR, Interrupt Vector: %d\n", interrupt_vector);
    __asm__ volatile("hlt\n");
}
