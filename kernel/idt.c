#include "idt.h"
#include "vga.h"

struct idt_entry
{
    unsigned short base_low;
    unsigned short selector;
    unsigned char  zero;
    unsigned char  flags;
    unsigned short base_high;
} __attribute__((packed));

struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

static struct idt_entry idt[256];
static struct idt_ptr idtp;

extern void idt_load(unsigned int);

extern void isr0(void);
extern void isr6(void);
extern void isr13(void);
extern void isr14(void);

static void idt_set_gate(
    unsigned char num,
    unsigned int base,
    unsigned short selector,
    unsigned char flags
)
{
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;

    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].flags = flags;
}

void idt_init(void)
{
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (unsigned int)&idt;

    for (int i = 0; i < 256; i++)
    {
        idt_set_gate(i, 0, 0x08, 0x8E);
    }

    idt_set_gate(0,  (unsigned int)isr0,  0x08, 0x8E);
    idt_set_gate(6,  (unsigned int)isr6,  0x08, 0x8E);
    idt_set_gate(13, (unsigned int)isr13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned int)isr14, 0x08, 0x8E);

    idt_load((unsigned int)&idtp);

    vga_write("IDT initialized!\n");
}