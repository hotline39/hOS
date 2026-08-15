#include "vga.h"

#define VGA_MEMORY ((volatile uint16_t*)0xB8000)

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint8_t color = 0x07;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static uint16_t vga_entry(char c)
{
    return ((uint16_t)color << 8) | (uint8_t)c;
}

void vga_init(void)
{
    vga_clear();
}

void vga_clear(void)
{
    for (uint32_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (uint32_t x = 0; x < VGA_WIDTH; x++)
        {
            VGA_MEMORY[y * VGA_WIDTH + x] = vga_entry(' ');
        }
    }

    cursor_x = 0;
    cursor_y = 0;
}

void vga_put_char(char c)
{
    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    else
    {
        VGA_MEMORY[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(c);
        cursor_x++;

        if (cursor_x >= VGA_WIDTH)
        {
            cursor_x = 0;
            cursor_y++;
        }
    }

    if (cursor_y >= VGA_HEIGHT)
    {
        cursor_y = 0;
    }
}

void vga_write(const char* str)
{
    while (*str)
    {
        vga_put_char(*str);
        str++;
    }
}

void vga_write_hex(uint32_t value)
{
    const char* hex = "0123456789ABCDEF";

    vga_write("0x");

    for (int i = 7; i >= 0; i--)
    {
        vga_put_char(hex[(value >> (i * 4)) & 0xF]);
    }
}

void vga_write_dec(uint32_t value)
{
    char buffer[11];
    int i = 0;

    if (value == 0)
    {
        vga_put_char('0');
        return;
    }

    while (value > 0)
    {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (i > 0)
    {
        vga_put_char(buffer[--i]);
    }
}