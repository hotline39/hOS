bits 32

global process_start_first

process_start_first:
    cli

    mov eax, [esp + 4]
    mov esp, eax

    popad
    iretd