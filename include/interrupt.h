#ifndef INTERRUPT_H
#define INTERRUPT_H

void exception_handler(unsigned int number);

void exception_handler_debug(
    unsigned int number,
    unsigned int error_code,
    unsigned int eip,
    unsigned int cs,
    unsigned int eflags
);

unsigned int irq_handler(
    unsigned int irq,
    unsigned int current_esp
);

#endif