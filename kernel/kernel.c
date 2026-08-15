#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "timer.h"
#include "keyboard.h"
#include "shell.h"
#include "pmm.h"
#include "paging.h"

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

    pic_remap();
    vga_write("PIC remapped!\n");

    timer_init(100);

    keyboard_init();

    pmm_init();

    paging_init();

    pic_unmask_irq(0);
    pic_unmask_irq(1);

    __asm__ volatile ("sti");

    vga_write("Interrupts enabled!\n");

    shell_init();

    while (1)
    {
        __asm__ volatile ("hlt");
    }
}