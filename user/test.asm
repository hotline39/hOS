bits 32

global _start

section .text

_start:
    mov eax, 1
    mov ebx, message
    int 0x80

    mov eax, 4
    int 0x80

.loop:
    jmp .loop

section .rodata

message:
    db "Hello from ELF User Program!", 10, 0