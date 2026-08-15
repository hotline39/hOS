#include "interrupt.h"
#include "vga.h"
#include "pic.h"
#include "timer.h"
#include "keyboard.h"

static const char* exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating Point",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating Point",
    "Virtualization",
    "Control Protection",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor Injection",
    "VMM Communication",
    "Security Exception",
    "Reserved"
};

void exception_handler(unsigned int number)
{
    vga_write("\n\n!!! KERNEL EXCEPTION !!!\n");
    vga_write("Exception: ");

    if (number < 32)
    {
        vga_write(exception_messages[number]);
    }
    else
    {
        vga_write("Unknown");
    }

    vga_write("\nSystem halted.\n");

    while (1)
    {
        __asm__ volatile ("cli");
        __asm__ volatile ("hlt");
    }
}

void irq_handler(unsigned int irq)
{
    if (irq == 0)
    {
        timer_handler();
    }
    else if (irq == 1)
    {
        keyboard_handler();
    }

    pic_send_eoi((unsigned char)irq);
}