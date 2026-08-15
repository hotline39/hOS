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
    void *stack;
    unsigned int stack_size;
    unsigned int esp;
    unsigned int ebp;
    unsigned int eip;
    struct process *next;
} process_t;

void process_init(void);
process_t *process_create(void);
void process_add(process_t *process);
process_t *process_next(void);
process_t *process_current(void);

#endif