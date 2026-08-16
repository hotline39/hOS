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
#include "elf.h"
#include "syscall.h"

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

    syscall_init();

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

    process_init();
    scheduler_init();

    ramfs_init();
    fat12_init();

    pic_unmask_irq(0);
    pic_unmask_irq(1);

    __asm__ volatile ("sti");

    vga_write("Interrupts enabled!\n");

    shell_init();

    while (1)
    {
        if (shell_run_pending())
        {
            const char *filename;
            const char *elf_data;
            unsigned int entry;

            filename = shell_run_file();

            elf_data = fat12_read(filename);

            if (elf_data == 0)
            {
                vga_write("File not found.\n");
                shell_run_clear();
                shell_prompt_show();
                continue;
            }

            if (!elf_check((const unsigned char *)elf_data))
            {
                vga_write("Invalid ELF file.\n");
                shell_run_clear();
                shell_prompt_show();
                continue;
            }

            entry = elf_load((const unsigned char *)elf_data);

            if (entry == 0)
            {
                vga_write("ELF load failed.\n");
                shell_run_clear();
                shell_prompt_show();
                continue;
            }

            shell_run_clear();

            enter_user_mode(entry);

            vga_putc('\n');
            shell_prompt_show();
        }

        __asm__ volatile ("hlt");
    }
}