bits 32

global _start

section .text

_start:
.loop:
    mov eax, 1
    mov ebx, message
    int 0x80
    jmp .loop

section .rodata

message:
    db "A", 0