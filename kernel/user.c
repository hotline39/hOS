#include "user.h"

static void user_write(const char *str)
{
    __asm__ volatile (
        "mov $1, %%eax\n"
        "mov %0, %%ebx\n"
        "int $0x80\n"
        :
        : "r"(str)
        : "eax", "ebx"
    );
}

void user_test(void)
{
    unsigned short cs;
    unsigned short ds;
    unsigned short ss;

    __asm__ volatile ("mov %%cs, %0" : "=r"(cs));
    __asm__ volatile ("mov %%ds, %0" : "=r"(ds));
    __asm__ volatile ("mov %%ss, %0" : "=r"(ss));

    if ((cs & 3) == 3 && (ds & 3) == 3 && (ss & 3) == 3)
        user_write("Ring 3 OK\n");
    else
        user_write("Ring 3 FAIL\n");

    while (1)
    {
    }
}