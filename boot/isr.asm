bits 32

global isr0
global isr6
global isr13
global isr14
global isr128


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
extern syscall_handler
extern exception_handler_debug

isr0:
    cli
    pushad

    push 0
    call exception_handler
    add esp, 4

    popad
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

    mov eax, [esp + 32]
    mov ebx, [esp + 36]
    mov ecx, [esp + 40]
    mov edx, [esp + 44]

    push edx
    push ecx
    push ebx
    push eax
    push 13

    call exception_handler_debug

    add esp, 20

.halt:
    cli
    hlt
    jmp .halt

isr14:
    cli
    pushad

    push 14
    call exception_handler
    add esp, 4

    popad
    sti
    iretd

irq0:
    cli
    pushad

    push esp
    push 0
    call irq_handler
    add esp, 8

    mov esp, eax

    popad
    sti
    iretd

irq1:
    cli
    pushad

    mov eax, esp

    push eax
    push 1
    call irq_handler
    add esp, 8

    mov esp, eax

    popad
    iretd

irq2:
    cli
    pushad

    mov eax, esp

    push eax
    push 2
    call irq_handler
    add esp, 8

    mov esp, eax

    popad
    iretd

irq3:
    cli
    pushad

    mov eax, esp

    push eax
    push 3
    call irq_handler
    add esp, 8

    mov esp, eax

    popad
    iretd

irq4:
    cli
    pushad

    mov eax, esp

    push eax
    push 4
    call irq_handler
    add esp, 8

    mov esp, eax

    popad
    iretd

irq5:
    cli
    pushad

    mov eax, esp

    push eax
    push 5
    call irq_handler
    add esp, 8

    mov esp, eax

    popad
    iretd

irq6:
    cli
    pushad

    mov eax, esp

    push eax
    push 6
    call irq_handler
    add esp, 8

    mov esp, eax

    popad
    iretd

irq7:
    cli
    pushad

    mov eax, esp

    push eax
    push 7
    call irq_handler
    add esp, 8

    mov esp, eax

    popad
    iretd

irq8:
    cli
    pushad

    mov eax, esp

    push eax
    push 8
    call irq_handler
    add esp, 8

    mov esp, eax

    popad
    iretd

irq9:
    cli
    pushad

    mov eax, esp

    push eax
    push 9
    call irq_handler
    add esp, 8

    mov esp, eax

    popad
    iretd

irq10:
    cli
    pushad

    mov eax, esp

    push eax
    push 10
    call irq_handler
    add esp, 8

    mov esp, eax

    popad
    iretd

irq11:
    cli
    pushad

    mov eax, esp

    push eax
    push 11
    call irq_handler
    add esp, 8

    mov esp, eax

    popad
    iretd

irq12:
    cli
    pushad

    mov eax, esp

    push eax
    push 12
    call irq_handler
    add esp, 8

    mov esp, eax

    popad
    iretd

irq13:
    cli
    pushad

    mov eax, esp

    push eax
    push 13
    call irq_handler
    add esp, 8

    mov esp, eax

    popad
    iretd

irq14:
    cli
    pushad

    mov eax, esp

    push eax
    push 14
    call irq_handler
    add esp, 8

    mov esp, eax

    popad
    iretd

irq15:
    cli
    pushad

    mov eax, esp

    push eax
    push 15
    call irq_handler
    add esp, 8

    mov esp, eax

    popad
    iretd

isr128:
    cli
    pushad
    push esp
    call syscall_handler
    add esp, 4
    popad
    iretd