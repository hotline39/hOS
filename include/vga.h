#ifndef VGA_H
#define VGA_H

#include <stdint.h>

void vga_init(void);
void vga_clear(void);
void vga_put_char(char c);
void vga_write(const char* str);
void vga_write_hex(uint32_t value);
void vga_write_dec(uint32_t value);

#endif