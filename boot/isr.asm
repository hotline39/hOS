bits 32

global isr0
global isr6
global isr13
global isr14

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

extern exception_handler
extern irq_handler


; ==============================
; CPU Exceptions
; ==============================

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


; ==============================
; Hardware IRQs
; ==============================

irq0:
    cli
    pushad

    push 0
    call irq_handler
    add esp, 4

    popad
    sti
    iretd


irq1:
    cli
    pushad

    push 1
    call irq_handler
    add esp, 4

    popad
    sti
    iretd


irq2:
    cli
    pushad

    push 2
    call irq_handler
    add esp, 4

    popad
    sti
    iretd


irq3:
    cli
    pushad

    push 3
    call irq_handler
    add esp, 4

    popad
    sti
    iretd


irq4:
    cli
    pushad

    push 4
    call irq_handler
    add esp, 4

    popad
    sti
    iretd


irq5:
    cli
    pushad

    push 5
    call irq_handler
    add esp, 4

    popad
    sti
    iretd


irq6:
    cli
    pushad

    push 6
    call irq_handler
    add esp, 4

    popad
    sti
    iretd


irq7:
    cli
    pushad

    push 7
    call irq_handler
    add esp, 4

    popad
    sti
    iretd


irq8:
    cli
    pushad

    push 8
    call irq_handler
    add esp, 4

    popad
    sti
    iretd


irq9:
    cli
    pushad

    push 9
    call irq_handler
    add esp, 4

    popad
    sti
    iretd


irq10:
    cli
    pushad

    push 10
    call irq_handler
    add esp, 4

    popad
    sti
    iretd


irq11:
    cli
    pushad

    push 11
    call irq_handler
    add esp, 4

    popad
    sti
    iretd


irq12:
    cli
    pushad

    push 12
    call irq_handler
    add esp, 4

    popad
    sti
    iretd


irq13:
    cli
    pushad

    push 13
    call irq_handler
    add esp, 4

    popad
    sti
    iretd


irq14:
    cli
    pushad

    push 14
    call irq_handler
    add esp, 4

    popad
    sti
    iretd


irq15:
    cli
    pushad

    push 15
    call irq_handler
    add esp, 4

    popad
    sti
    iretd