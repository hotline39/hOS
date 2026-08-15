#include "timer.h"

#define PIT_COMMAND 0x43
#define PIT_CHANNEL0 0x40
#define PIT_BASE_FREQUENCY 1193180

static volatile unsigned int ticks = 0;

static inline void outb(unsigned short port, unsigned char value)
{
    __asm__ volatile (
        "outb %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}

void timer_init(unsigned int frequency)
{
    unsigned int divisor;

    if (frequency == 0)
        return;

    divisor = PIT_BASE_FREQUENCY / frequency;

    outb(PIT_COMMAND, 0x36);

    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
}

void timer_handler(void)
{
    ticks++;
}

unsigned int timer_get_ticks(void)
{
    return ticks;
}