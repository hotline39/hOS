#include "gdt.h"

struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

struct gdt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct tss_entry
{
    unsigned int prev_tss;
    unsigned int esp0;
    unsigned int ss0;
    unsigned int esp1;
    unsigned int ss1;
    unsigned int esp2;
    unsigned int ss2;
    unsigned int cr3;
    unsigned int eip;
    unsigned int eflags;
    unsigned int eax;
    unsigned int ecx;
    unsigned int edx;
    unsigned int ebx;
    unsigned int esp;
    unsigned int ebp;
    unsigned int esi;
    unsigned int edi;
    unsigned int es;
    unsigned int cs;
    unsigned int ss;
    unsigned int ds;
    unsigned int fs;
    unsigned int gs;
    unsigned int ldt;
    unsigned short trap;
    unsigned short iomap_base;
} __attribute__((packed));

static struct gdt_entry gdt[6];
static struct gdt_ptr gp;
static struct tss_entry tss;
static unsigned char tss_stack[4096];

extern void gdt_flush(unsigned int);

static void gdt_set_gate(
    int num,
    unsigned int base,
    unsigned int limit,
    unsigned char access,
    unsigned char granularity
)
{
    gdt[num].base_low = base & 0xFFFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = limit & 0xFFFF;
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= granularity & 0xF0;

    gdt[num].access = access;
}

static void tss_init(void)
{
    unsigned int base;
    unsigned int limit;

    base = (unsigned int)&tss;
    limit = sizeof(tss) - 1;

    gdt_set_gate(5, base, limit, 0x89, 0x40);

    tss.prev_tss = 0;
    tss.esp0 = (unsigned int)tss_stack + sizeof(tss_stack);
    tss.ss0 = 0x10;
    tss.esp1 = 0;
    tss.ss1 = 0;
    tss.esp2 = 0;
    tss.ss2 = 0;
    tss.cr3 = 0;
    tss.eip = 0;
    tss.eflags = 0x202;
    tss.eax = 0;
    tss.ecx = 0;
    tss.edx = 0;
    tss.ebx = 0;
    tss.esp = 0;
    tss.ebp = 0;
    tss.esi = 0;
    tss.edi = 0;
    tss.es = 0x10;
    tss.cs = 0x08;
    tss.ss = 0x10;
    tss.ds = 0x10;
    tss.fs = 0x10;
    tss.gs = 0x10;
    tss.ldt = 0;
    tss.trap = 0;
    tss.iomap_base = sizeof(tss);
}

void gdt_init(void)
{
    gp.limit = sizeof(gdt) - 1;
    gp.base = (unsigned int)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);

    gdt_set_gate(
        1,
        0,
        0xFFFFFFFF,
        0x9A,
        0xCF
    );

    gdt_set_gate(
        2,
        0,
        0xFFFFFFFF,
        0x92,
        0xCF
    );

    gdt_set_gate(
        3,
        0,
        0xFFFFFFFF,
        0xFA,
        0xCF
    );

    gdt_set_gate(
        4,
        0,
        0xFFFFFFFF,
        0xF2,
        0xCF
    );

    tss_init();

    gdt_flush((unsigned int)&gp);
}

void tss_set_kernel_stack(unsigned int esp0)
{
    tss.esp0 = esp0;
}