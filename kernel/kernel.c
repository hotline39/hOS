#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"

void kernel_main(void)
{
    vga_init();

    vga_write("==============================\n");
    vga_write("            hOS\n");
    vga_write("==============================\n\n");

    vga_write("Kernel initialized!\n");

    gdt_init();
    vga_write("GDT initialized!\n");

    idt_init();
    vga_write("IDT initialized!\n");

    pic_remap();
    vga_write("PIC remapped!\n");

    while (1)
    {
        __asm__ volatile ("hlt");
    }
}