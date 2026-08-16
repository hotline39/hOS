#include "process.h"
#include "heap.h"
#include "vga.h"

#define PROCESS_KERNEL_STACK_SIZE 4096

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

process_t *process_create_user(
    unsigned int entry,
    unsigned int user_stack_top
)
{
    process_t *process;
    unsigned int *stack;

    process = (process_t *)kmalloc(sizeof(process_t));

    if (process == 0)
        return 0;

    process->kernel_stack = kmalloc(PROCESS_KERNEL_STACK_SIZE);

    if (process->kernel_stack == 0)
        return 0;

    process->pid = next_pid++;
    process->state = PROCESS_READY;

    process->kernel_stack_size = PROCESS_KERNEL_STACK_SIZE;
    process->kernel_stack_top =
        (unsigned int)process->kernel_stack +
        PROCESS_KERNEL_STACK_SIZE;

    process->user_stack_top = user_stack_top;

    process->eip = entry;
    process->next = 0;

    stack = (unsigned int *)process->kernel_stack_top;

    *(--stack) = 0x23;
    *(--stack) = user_stack_top;
    *(--stack) = 0x202;
    *(--stack) = 0x1B;
    *(--stack) = entry;

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