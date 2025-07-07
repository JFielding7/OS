#ifndef IDT_H
#define IDT_H

#define IDT_SIZE 256

/* Type Attributes */
#define IDT_PRESENT_FLAG (1 << 7)

#define IDT_RING0 0
#define IDT_RING3 3

#define IDT_INTERRUPT_GATE_TYPE 0xE
#define IDT_TRAP_GATE_TYPE      0xF

#define IDT_TYPE_ATTRIBUTES(dpl, gate_type) (IDT_PRESENT_FLAG | (dpl << 5) | gate_type)

#define IDT_DEFAULT_TYPE_ATTRIBUTES IDT_TYPE_ATTRIBUTES(IDT_RING0, IDT_INTERRUPT_GATE_TYPE)

void idt_init();

#endif //IDT_H
