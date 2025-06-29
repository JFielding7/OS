BITS 16
org 0x7C00


start:
    mov si, optimal_question
    call print_si
    call get_line
    mov si, optimal_answer
    call print_si
    call print_new_line
    cli
    hlt


get_line_loop:
    cmp al, 0x0D
    jnz print_char
    call print_new_line
    ret

print_char:
    call print_al

get_line:
    mov ah, 0x00
    int 0x16
    jmp get_line_loop


print_si:
    lodsb
    or al, al
    jnz print_curr
    ret
print_curr:
    call print_al
    jmp print_si


print_new_line:
    mov al, 0x0D
    call print_al
    mov al, 0x0A
    call print_al
    ret


print_al:
    mov ah, 0x0E
    mov bh, 0x00
    mov bl, 0x07
    int 0x10
    ret


optimal_question db 'Who Won? ', 0
optimal_answer db 'DK Won of course, he is optimal!', 0


times 510-($-$$) db 0
dw 0xAA55
