#include "scheduler.h"
#include "process.h"
#include "vga.h"

static unsigned int scheduler_ticks = 0;

void scheduler_init(void)
{
    scheduler_ticks = 0;
    vga_write("Scheduler initialized!\n");
}

void scheduler_tick(void)
{
    scheduler_ticks++;

    if (scheduler_ticks < 10)
        return;

    scheduler_ticks = 0;

    process_t *next = process_next();

    if (next == 0)
        return;
}