bits 32

global enter_user_mode
global leave_user_mode

section .data

kernel_saved_esp:
    dd 0

section .text

enter_user_mode:
    mov [kernel_saved_esp], esp
    mov edx, [esp + 4]

    cli

    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push 0x23
    push 0x2F0000
    push 0x202
    push 0x1B
    push edx

    iretd

leave_user_mode:
    cli

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov esp, [kernel_saved_esp]

    sti
    ret