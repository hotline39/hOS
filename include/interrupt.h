#ifndef INTERRUPT_H
#define INTERRUPT_H

void exception_handler(unsigned int number);
unsigned int irq_handler(unsigned int irq, unsigned int current_esp);

#endif