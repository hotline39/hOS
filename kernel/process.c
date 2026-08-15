#include "process.h"
#include "heap.h"
#include "vga.h"

#define PROCESS_STACK_SIZE 4096

static unsigned int next_pid = 1;
static process_t *process_head = 0;
static process_t *current_process = 0;

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

    process = (process_t *)kmalloc(sizeof(process_t));

    if (process == 0)
        return 0;

    process->stack = kmalloc(PROCESS_STACK_SIZE);

    if (process->stack == 0)
        return 0;

    process->pid = next_pid++;
    process->state = PROCESS_READY;
    process->stack_size = PROCESS_STACK_SIZE;
    process->esp = (unsigned int)process->stack + PROCESS_STACK_SIZE;
    process->ebp = process->esp;
    process->eip = 0;
    process->next = 0;

    process_add(process);

    return process;
}

void process_add(process_t *process)
{
    if (process_head == 0)
    {
        process_head = process;
        current_process = process;
        return;
    }

    process_t *current = process_head;

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