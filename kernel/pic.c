#include "pic.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21

#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define ICW1_INIT     0x10
#define ICW1_ICW4     0x01

#define ICW4_8086     0x01

static inline void outb(unsigned short port, unsigned char value)
{
    __asm__ volatile (
        "outb %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}

static inline unsigned char inb(unsigned short port)
{
    unsigned char value;

    __asm__ volatile (
        "inb %1, %0"
        : "=a"(value)
        : "Nd"(port)
    );

    return value;
}

void pic_remap(void)
{
    unsigned char master_mask = inb(PIC1_DATA);
    unsigned char slave_mask  = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    // Master PIC → IDT 32~39
    outb(PIC1_DATA, 0x20);

    // Slave PIC → IDT 40~47
    outb(PIC2_DATA, 0x28);

    // Master IRQ2 → Slave PIC
    outb(PIC1_DATA, 0x04);

    // Slave PIC cascade identity
    outb(PIC2_DATA, 0x02);

    // 8086 mode
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    // 기존 mask 복원
    outb(PIC1_DATA, master_mask);
    outb(PIC2_DATA, slave_mask);
}

void pic_send_eoi(unsigned char irq)
{
    if (irq >= 8)
    {
        outb(PIC2_COMMAND, 0x20);
    }

    outb(PIC1_COMMAND, 0x20);
}