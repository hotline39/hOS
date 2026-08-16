#include "syscall.h"
#include "vga.h"

void syscall_init(void)
{
    vga_write("System Call initialized!\n");
}

void syscall_handler(void)
{
    vga_write("System Call invoked!\n");
}