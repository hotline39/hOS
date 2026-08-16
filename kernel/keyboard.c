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

static const char shift_keymap[128] =
{
    0,
    27,
    '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
    '_', '+',
    '\b',
    '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
    '{', '}',
    '\n',
    0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
    ':', '"', '~',
    0,
    '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M',
    '<', '>', '?',
    0,
    '*',
    0,
    ' '
};

static int shift_pressed = 0;

void keyboard_init(void)
{
    shift_pressed = 0;
}

void keyboard_handler(void)
{
    unsigned char scancode;
    char c;

    scancode = inb(KEYBOARD_DATA_PORT);

    if (scancode == 0x2A || scancode == 0x36)
    {
        shift_pressed = 1;
        return;
    }

    if (scancode == 0xAA || scancode == 0xB6)
    {
        shift_pressed = 0;
        return;
    }

    if (scancode & 0x80)
    {
        return;
    }

    if (scancode >= 128)
    {
        return;
    }

    if (shift_pressed)
    {
        c = shift_keymap[scancode];
    }
    else
    {
        c = keymap[scancode];
    }

    if (c == 0)
    {
        return;
    }

    shell_putchar(c);
}