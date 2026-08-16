#ifndef SCHEDULER_H
#define SCHEDULER_H

void scheduler_init(void);
void scheduler_enable(void);
void scheduler_disable(void);
int scheduler_is_enabled(void);

unsigned int scheduler_tick(unsigned int current_esp);

#endif