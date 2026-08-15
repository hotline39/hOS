#include "vga.h"

#define VGA_MEMORY ((volatile unsigned short*)0xB8000)

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static unsigned int cursor_row = 0;
static unsigned int cursor_col = 0;

static unsigned char color = 0x07;

static void vga_scroll(void)
{
    if (cursor_row < VGA_HEIGHT)
    {
        return;
    }

    for (unsigned int row = 1; row < VGA_HEIGHT; row++)
    {
        for (unsigned int col = 0; col < VGA_WIDTH; col++)
        {
            VGA_MEMORY[(row - 1) * VGA_WIDTH + col] =
                VGA_MEMORY[row * VGA_WIDTH + col];
        }
    }

    for (unsigned int col = 0; col < VGA_WIDTH; col++)
    {
        VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + col] =
            ((unsigned short)color << 8) | ' ';
    }

    cursor_row = VGA_HEIGHT - 1;
}

static void vga_update_cursor(void)
{
    unsigned short position;

    position = cursor_row * VGA_WIDTH + cursor_col;

    __asm__ volatile (
        "movb $0x0F, %%al\n"
        "outb %%al, $0x3D4\n"
        "movb %b0, %%al\n"
        "outb %%al, $0x3D5\n"
        "movb $0x0E, %%al\n"
        "outb %%al, $0x3D4\n"
        "movb %b1, %%al\n"
        "outb %%al, $0x3D5\n"
        :
        : "q"((unsigned char)(position & 0xFF)),
          "q"((unsigned char)((position >> 8) & 0xFF))
        : "eax"
    );
}

void vga_init(void)
{
    cursor_row = 0;
    cursor_col = 0;

    for (unsigned int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
    {
        VGA_MEMORY[i] =
            ((unsigned short)color << 8) | ' ';
    }

    vga_update_cursor();
}

void vga_putc(char c)
{
    if (c == '\n')
    {
        cursor_col = 0;
        cursor_row++;

        vga_scroll();
        vga_update_cursor();

        return;
    }

    if (c == '\b')
    {
        vga_backspace();
        return;
    }

    VGA_MEMORY[cursor_row * VGA_WIDTH + cursor_col] =
        ((unsigned short)color << 8) | (unsigned char)c;

    cursor_col++;

    if (cursor_col >= VGA_WIDTH)
    {
        cursor_col = 0;
        cursor_row++;
    }

    vga_scroll();
    vga_update_cursor();
}

void vga_backspace(void)
{
    if (cursor_col == 0)
    {
        if (cursor_row == 0)
        {
            return;
        }

        cursor_row--;
        cursor_col = VGA_WIDTH - 1;
    }
    else
    {
        cursor_col--;
    }

    VGA_MEMORY[cursor_row * VGA_WIDTH + cursor_col] =
        ((unsigned short)color << 8) | ' ';

    vga_update_cursor();
}

void vga_write(const char* str)
{
    while (*str)
    {
        vga_putc(*str);
        str++;
    }
}