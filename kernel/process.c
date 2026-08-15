#include "process.h"
#include "heap.h"
#include "vga.h"

#define PROCESS_STACK_SIZE 4096

static unsigned int next_pid = 1;
static process_t *process_head = 0;
static process_t *current_process = 0;

static void process_entry(void)
{
    while (1)
    {
        vga_write("P");
    }
}

void process_init(void)
{
    next_pid = 1;
    process_head = 0;
    current_process = 0;

    vga_write("Process subsystem initialized!\n");
}

process_t *process_create(void)
{
    process_t *process;
    unsigned int *stack;

    process = (process_t *)kmalloc(sizeof(process_t));

    if (process == 0)
        return 0;

    process->stack = kmalloc(PROCESS_STACK_SIZE);

    if (process->stack == 0)
        return 0;

    process->pid = next_pid++;
    process->state = PROCESS_READY;
    process->stack_size = PROCESS_STACK_SIZE;
    process->ebp = (unsigned int)process->stack + PROCESS_STACK_SIZE;
    process->eip = (unsigned int)process_entry;
    process->next = 0;

    stack = (unsigned int *)((unsigned int)process->stack + PROCESS_STACK_SIZE);

    *(--stack) = 0x202;
    *(--stack) = 0x08;
    *(--stack) = (unsigned int)process_entry;

    *(--stack) = 0;
    *(--stack) = 0;
    *(--stack) = 0;
    *(--stack) = 0;
    *(--stack) = 0;
    *(--stack) = 0;
    *(--stack) = 0;
    *(--stack) = 0;

    process->esp = (unsigned int)stack;

    process_add(process);

    return process;
}

void process_add(process_t *process)
{
    process_t *current;

    if (process_head == 0)
    {
        process_head = process;
        current_process = process;
        return;
    }

    current = process_head;

    while (current->next != 0)
        current = current->next;

    current->next = process;
}

process_t *process_next(void)
{
    if (current_process == 0)
        return 0;

    if (current_process->next != 0)
        current_process = current_process->next;
    else
        current_process = process_head;

    return current_process;
}

process_t *process_current(void)
{
    return current_process;
}