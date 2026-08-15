#include "scheduler.h"
#include "process.h"

static unsigned int scheduler_ticks = 0;

void scheduler_init(void)
{
    scheduler_ticks = 0;
}

unsigned int scheduler_tick(unsigned int current_esp)
{
    process_t *current;
    process_t *next;

    scheduler_ticks++;

    if (scheduler_ticks < 10)
        return current_esp;

    scheduler_ticks = 0;

    current = process_current();

    if (current == 0)
        return current_esp;

    current->esp = current_esp;

    next = process_next();

    if (next == 0 || next == current)
        return current_esp;

    current->state = PROCESS_READY;
    next->state = PROCESS_RUNNING;

    return next->esp;
}