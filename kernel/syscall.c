#include "syscall.h"
#include "vga.h"
#include "process.h"
#include "user.h"

void syscall_init(void)
{
    vga_write("System Call initialized!\n");
}

void syscall_handler(unsigned int *regs)
{
    unsigned int syscall_number = regs[7];
    process_t *current;

    switch (syscall_number)
    {
        case SYS_WRITE:
            vga_write((char *)regs[4]);
            regs[7] = 0;
            break;

        case SYS_GETPID:
            current = process_current();

            if (current != 0)
                regs[7] = current->pid;
            else
                regs[7] = 0;

            break;

        case SYS_YIELD:
            current = process_current();

            if (current != 0)
                current->state = PROCESS_READY;

            regs[7] = 0;
            break;

        case SYS_EXIT:
            leave_user_mode();
            break;

        default:
            vga_write("Unknown System Call!\n");
            regs[7] = (unsigned int)-1;
            break;
    }
}