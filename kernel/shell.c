#include "shell.h"
#include "vga.h"
#include "ramfs.h"

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

static unsigned int string_length(const char *str)
{
    unsigned int length = 0;

    while (str[length] != '\0')
        length++;

    return length;
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
    vga_write("  help  - show this help\n");
    vga_write("  clear - clear the screen\n");
    vga_write("  echo  - print text\n");
    vga_write("  info  - show system information\n");
    vga_write("  ls    - list files\n");
    vga_write("  touch - create file\n");
    vga_write("  cat   - read file\n");
    vga_write("  write - write file\n");
}

static void shell_info(void)
{
    vga_write("hOS kernel\n");
    vga_write("Architecture: i386\n");
    vga_write("Interrupts: PIC / IRQ\n");
    vga_write("Timer: PIT\n");
    vga_write("Keyboard: PS/2\n");
    vga_write("Filesystem: RAM FS\n");
}

static void shell_ls(void)
{
    ramfs_list();
}

static void shell_touch(const char *args)
{
    unsigned int i = 0;
    char filename[RAMFS_FILENAME_SIZE];

    while (args[i] != '\0' && args[i] != ' ')
    {
        if (i >= RAMFS_FILENAME_SIZE - 1)
        {
            vga_write("Filename too long.\n");
            return;
        }

        filename[i] = args[i];
        i++;
    }

    filename[i] = '\0';

    if (filename[0] == '\0' || args[i] != '\0')
    {
        vga_write("Usage: touch <file>\n");
        return;
    }

    if (ramfs_create(filename) == 0)
    {
        vga_write("File created.\n");
    }
    else
    {
        vga_write("Failed to create file.\n");
    }
}

static void shell_cat(const char *args)
{
    unsigned int i = 0;
    char filename[RAMFS_FILENAME_SIZE];
    const char *data;

    while (args[i] != '\0' && args[i] != ' ')
    {
        if (i >= RAMFS_FILENAME_SIZE - 1)
        {
            vga_write("Filename too long.\n");
            return;
        }

        filename[i] = args[i];
        i++;
    }

    filename[i] = '\0';

    if (filename[0] == '\0' || args[i] != '\0')
    {
        vga_write("Usage: cat <file>\n");
        return;
    }

    data = ramfs_read(filename);

    if (data == 0)
    {
        vga_write("File not found.\n");
        return;
    }

    vga_write(data);
    vga_putc('\n');
}

static void shell_write(const char *args)
{
    unsigned int i = 0;
    unsigned int filename_length = 0;
    char filename[RAMFS_FILENAME_SIZE];
    const char *data;

    while (args[i] != '\0' && args[i] != ' ')
        i++;

    filename_length = i;

    if (filename_length == 0 || args[i] == '\0')
    {
        vga_write("Usage: write <file> <text>\n");
        return;
    }

    if (filename_length >= RAMFS_FILENAME_SIZE)
    {
        vga_write("Filename too long.\n");
        return;
    }

    while (i > 0)
    {
        filename[i - 1] = args[i - 1];
        i--;
    }

    filename[filename_length] = '\0';

    data = args + filename_length + 1;

    if (*data == '\0')
    {
        vga_write("Usage: write <file> <text>\n");
        return;
    }

    if (ramfs_write(filename, data) == 0)
    {
        vga_write("File written.\n");
    }
    else
    {
        vga_write("File not found.\n");
    }
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
        return;

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
    else if (string_equal(buffer, "ls"))
    {
        shell_ls();
    }
    else if (string_equal(buffer, "touch"))
    {
        vga_write("Usage: touch <file>\n");
    }
    else if (string_starts_with(buffer, "touch "))
    {
        shell_touch(buffer + 6);
    }
    else if (string_equal(buffer, "cat"))
    {
        vga_write("Usage: cat <file>\n");
    }
    else if (string_starts_with(buffer, "cat "))
    {
        shell_cat(buffer + 4);
    }
    else if (string_equal(buffer, "write"))
    {
        vga_write("Usage: write <file> <text>\n");
    }
    else if (string_starts_with(buffer, "write "))
    {
        shell_write(buffer + 6);
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