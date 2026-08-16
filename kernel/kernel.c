#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "timer.h"
#include "keyboard.h"
#include "shell.h"
#include "pmm.h"
#include "paging.h"
#include "heap.h"
#include "process.h"
#include "scheduler.h"
#include "ramfs.h"
#include "fat12.h"
#include "user.h"

extern void enter_user_mode(void);

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

    timer_init(100);
    vga_write("Timer initialized!\n");

    keyboard_init();
    vga_write("Keyboard initialized!\n");

    pmm_init();
    vga_write("Physical memory initialized!\n");

    paging_init();
    vga_write("Paging initialized!\n");

    heap_init();

    pic_unmask_irq(0);
    pic_unmask_irq(1);

    process_init();

    scheduler_init();

    ramfs_init();

    fat12_init();

    pic_unmask_irq(0);
    pic_unmask_irq(1);

    __asm__ volatile ("sti");

    vga_write("Interrupts enabled!\n");

    shell_init();

    enter_user_mode();

    while (1)
    {
        __asm__ volatile ("hlt");
    }
}