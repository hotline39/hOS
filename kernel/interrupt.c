#include "interrupt.h"
#include "vga.h"
#include "pic.h"
#include "timer.h"
#include "keyboard.h"
#include "scheduler.h"

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
    "Hypervisor Injection",
    "VMM Communication",
    "Security Exception",
    "Reserved"
};

static void write_hex(unsigned int value)
{
    char hex[] = "0123456789ABCDEF";
    int i;

    vga_write("0x");

    for (i = 7; i >= 0; i--)
    {
        vga_putc(hex[(value >> (i * 4)) & 0xF]);
    }
}

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

void exception_handler_debug(
    unsigned int number,
    unsigned int error_code,
    unsigned int eip,
    unsigned int cs,
    unsigned int eflags
)
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

    vga_write("\nError: ");
    write_hex(error_code);

    vga_write("\nEIP: ");
    write_hex(eip);

    vga_write("\nCS: ");
    write_hex(cs);

    vga_write("\nEFLAGS: ");
    write_hex(eflags);

    vga_write("\nSystem halted.\n");

    while (1)
    {
        __asm__ volatile ("cli");
        __asm__ volatile ("hlt");
    }
}

unsigned int irq_handler(unsigned int irq, unsigned int current_esp)
{
    unsigned int next_esp;

    next_esp = current_esp;

    if (irq == 0)
    {
        timer_handler();
        next_esp = scheduler_tick(current_esp);
    }
    else if (irq == 1)
    {
        keyboard_handler();
    }

    pic_send_eoi((unsigned char)irq);

    return next_esp;
}