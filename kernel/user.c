#include "user.h"

void user_test(void)
{
    __asm__ volatile ("int $0x80");

    while (1)
    {
    }
}