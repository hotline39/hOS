#include "paging.h"
#include "vga.h"

#define PAGE_PRESENT    0x001
#define PAGE_WRITABLE   0x002
#define PAGE_USER       0x004

#define PAGE_SIZE       4096
#define PAGE_TABLE_SIZE 1024

static unsigned int page_directory[PAGE_TABLE_SIZE]
    __attribute__((aligned(PAGE_SIZE)));

static unsigned int first_page_table[PAGE_TABLE_SIZE]
    __attribute__((aligned(PAGE_SIZE)));

static unsigned int second_page_table[PAGE_TABLE_SIZE]
    __attribute__((aligned(PAGE_SIZE)));

static inline void load_page_directory(unsigned int address)
{
    __asm__ volatile (
        "mov %0, %%cr3"
        :
        : "r"(address)
        : "memory"
    );
}

static inline void enable_paging(void)
{
    unsigned int cr0;

    __asm__ volatile (
        "mov %%cr0, %0"
        : "=r"(cr0)
    );

    cr0 |= 0x80000000;

    __asm__ volatile (
        "mov %0, %%cr0"
        :
        : "r"(cr0)
        : "memory"
    );
}

void paging_init(void)
{
    unsigned int i;

    for (i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        page_directory[i] = 0;
    }

    for (i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        first_page_table[i] =
            (i * PAGE_SIZE) |
            PAGE_PRESENT |
            PAGE_WRITABLE |
            PAGE_USER;
    }

    for (i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        second_page_table[i] =
            (0x00400000 + i * PAGE_SIZE) |
            PAGE_PRESENT |
            PAGE_WRITABLE;
    }

    page_directory[0] =
        ((unsigned int)first_page_table) |
        PAGE_PRESENT |
        PAGE_WRITABLE |
        PAGE_USER;

    page_directory[1] =
        ((unsigned int)second_page_table) |
        PAGE_PRESENT |
        PAGE_WRITABLE;

    load_page_directory((unsigned int)page_directory);

    enable_paging();

    vga_write("Paging enabled!\n");
}