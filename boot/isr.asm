bits 32

global isr0
global isr6
global isr13
global isr14

extern exception_handler

isr0:
    cli
    pushad

    push 0
    call exception_handler
    add esp, 4

    popad
    sti
    iretd

isr6:
    cli
    pushad

    push 6
    call exception_handler
    add esp, 4

    popad
    sti
    iretd

isr13:
    cli
    pushad

    push 13
    call exception_handler
    add esp, 4

    popad
    sti
    iretd

isr14:
    cli
    pushad

    push 14
    call exception_handler
    add esp, 4

    popad
    sti
    iretd