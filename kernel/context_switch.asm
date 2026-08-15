bits 32

global process_switch

process_switch:
    push ebp
    push ebx
    push esi
    push edi

    mov eax, [esp + 20]
    mov [eax], esp

    mov eax, [esp + 24]
    mov esp, [eax]

    pop edi
    pop esi
    pop ebx
    pop ebp

    ret