#include "keyboard.h"
#include "shell.h"

#define KEYBOARD_DATA_PORT 0x60

static inline unsigned char inb(unsigned short port)
{
    unsigned char value;

    __asm__ volatile (
        "inb %1, %0"
        : "=a"(value)
        : "Nd"(port)
    );

    return value;
}

static const char keymap[128] =
{
    0,
    27,
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=',
    '\b',
    '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '[', ']',
    '\n',
    0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    ';', '\'', '`',
    0,
    '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm',
    ',', '.', '/',
    0,
    '*',
    0,
    ' '
};

void keyboard_init(void)
{
}

void keyboard_handler(void)
{
    unsigned char scancode;
    char c;

    scancode = inb(KEYBOARD_DATA_PORT);

    /*
     * Key release
     */
    if (scancode & 0x80)
    {
        return;
    }

    if (scancode >= 128)
    {
        return;
    }

    c = keymap[scancode];

    if (c == 0)
    {
        return;
    }

    shell_putchar(c);
}