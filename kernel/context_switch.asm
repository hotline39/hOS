bits 32

global process_switch

process_switch:
    mov eax, [esp + 4]
    mov edx, [esp + 8]

    push ebp
    push ebx
    push esi
    push edi

    mov [eax + 16], esp

    mov esp, [edx + 16]

    pop edi
    pop esi
    pop ebx
    pop ebp

    ret