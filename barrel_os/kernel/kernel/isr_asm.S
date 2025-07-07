    .section .text
    .global isr_stub_80
isr_stub_80:
    pusha
    call print_dk
    popa
    iret

    .global isr_stub_default
isr_stub_default:
    pusha
    call default_isr
    popa
    iret
