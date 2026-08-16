#ifndef PROCESS_H
#define PROCESS_H

typedef enum
{
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_TERMINATED
} process_state_t;

typedef struct process
{
    unsigned int pid;
    process_state_t state;

    void *kernel_stack;
    unsigned int kernel_stack_size;
    unsigned int kernel_stack_top;

    unsigned int user_stack_top;

    unsigned int esp;
    unsigned int eip;

    struct process *next;
} process_t;

void process_init(void);

process_t *process_create_user(
    unsigned int entry,
    unsigned int user_stack_top
);

void process_add(process_t *process);
process_t *process_next(void);
process_t *process_current(void);

void process_start_first(unsigned int esp);

#endif