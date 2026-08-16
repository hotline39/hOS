#include "scheduler.h"
#include "process.h"
#include "gdt.h"

static unsigned int scheduler_ticks = 0;
static int scheduler_enabled = 0;

void scheduler_init(void)
{
    scheduler_ticks = 0;
    scheduler_enabled = 0;
}

void scheduler_enable(void)
{
    process_t *current;

    scheduler_ticks = 0;
    scheduler_enabled = 1;

    current = process_current();

    if (current != 0)
    {
        current->state = PROCESS_RUNNING;
        tss_set_kernel_stack(current->kernel_stack_top);
    }
}

void scheduler_disable(void)
{
    scheduler_enabled = 0;
}

int scheduler_is_enabled(void)
{
    return scheduler_enabled;
}

unsigned int scheduler_tick(unsigned int current_esp)
{
    process_t *current;
    process_t *next;

    if (!scheduler_enabled)
        return current_esp;

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

    tss_set_kernel_stack(next->kernel_stack_top);

    return next->esp;
}