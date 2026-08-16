bits 32

global enter_user_mode
extern user_test

enter_user_mode:
    cli

    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push 0x23
    push 0x90000
    push 0x202
    push 0x1B
    push user_test

    iretd