#ifndef INTERRUPT_H
#define INTERRUPT_H

void exception_init(void);
void exception_handler(unsigned int number);

void irq_handler(unsigned int irq);

#endif