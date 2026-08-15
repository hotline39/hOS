#include "shell.h"
#include "vga.h"
#include "heap.h"
#include "process.h"

#define SHELL_BUFFER_SIZE 128

static char buffer[SHELL_BUFFER_SIZE];
static unsigned int buffer_length = 0;

static int string_equal(const char* a, const char* b)
{
    unsigned int i = 0;

    while (a[i] != '\0' && b[i] != '\0')
    {
        if (a[i] != b[i])
            return 0;

        i++;
    }

    return a[i] == '\0' && b[i] == '\0';
}

static int string_starts_with(const char* str, const char* prefix)
{
    unsigned int i = 0;

    while (prefix[i] != '\0')
    {
        if (str[i] != prefix[i])
            return 0;

        i++;
    }

    return 1;
}

static void shell_prompt(void)
{
    vga_write("hOS> ");
}

static void shell_clear(void)
{
    vga_init();
}

static void shell_help(void)
{
    vga_write("Available commands:\n");
    vga_write("  help    - show this help\n");
    vga_write("  clear   - clear the screen\n");
    vga_write("  echo    - print text\n");
    vga_write("  info    - show system information\n");
    vga_write("  heap    - test kernel heap\n");
    vga_write("  process - create a process\n");
}

static void shell_info(void)
{
    vga_write("hOS kernel\n");
    vga_write("Architecture: i386\n");
    vga_write("Interrupts: PIC / IRQ\n");
    vga_write("Timer: PIT\n");
    vga_write("Keyboard: PS/2\n");
}

static void shell_heap(void)
{
    void *a;
    void *b;

    a = kmalloc(64);
    b = kmalloc(128);

    if (a == 0 || b == 0)
    {
        vga_write("Heap allocation failed!\n");
        return;
    }

    vga_write("Heap allocation OK\n");

    kfree(a);
    kfree(b);

    vga_write("Heap free OK\n");
}

static void shell_process(void)
{
    process_t *process;

    process = process_create();

    if (process == 0)
    {
        vga_write("Process creation failed!\n");
        return;
    }

    vga_write("Process created!\n");
    vga_write("PID: ");

    if (process->pid == 1)
        vga_write("1\n");
    else if (process->pid == 2)
        vga_write("2\n");
    else if (process->pid == 3)
        vga_write("3\n");
    else
        vga_write("other\n");
}

void shell_init(void)
{
    buffer_length = 0;

    vga_write("\n");
    vga_write("Welcome to hOS!\n");
    vga_write("Type 'help' for available commands.\n\n");

    shell_prompt();
}

void shell_putchar(char c)
{
    if (c == '\n')
    {
        vga_putc('\n');
        shell_execute();
        return;
    }

    if (c == '\b')
    {
        if (buffer_length > 0)
        {
            buffer_length--;
            vga_backspace();
        }

        return;
    }

    if (buffer_length >= SHELL_BUFFER_SIZE - 1)
    {
        return;
    }

    buffer[buffer_length++] = c;
    buffer[buffer_length] = '\0';

    vga_putc(c);
}

void shell_execute(void)
{
    if (buffer_length == 0)
    {
        shell_prompt();
        return;
    }

    if (string_equal(buffer, "help"))
    {
        shell_help();
    }
    else if (string_equal(buffer, "clear"))
    {
        shell_clear();
    }
    else if (string_equal(buffer, "info"))
    {
        shell_info();
    }
    else if (string_equal(buffer, "heap"))
    {
        shell_heap();
    }
    else if (string_equal(buffer, "process"))
    {
        shell_process();
    }
    else if (string_starts_with(buffer, "echo "))
    {
        vga_write(buffer + 5);
        vga_putc('\n');
    }
    else if (string_equal(buffer, "echo"))
    {
        vga_putc('\n');
    }
    else
    {
        vga_write("Unknown command: ");
        vga_write(buffer);
        vga_putc('\n');
    }

    buffer_length = 0;
    buffer[0] = '\0';

    shell_prompt();
}