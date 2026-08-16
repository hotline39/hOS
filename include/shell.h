#ifndef SHELL_H
#define SHELL_H

void shell_init(void);
void shell_putchar(char c);
void shell_execute(void);

int shell_run_pending(void);
const char *shell_run_file(void);
void shell_run_clear(void);
void shell_prompt_show(void);

#endif