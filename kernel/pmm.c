#include "pmm.h"
#include "vga.h"

#define MEMORY_START 0x00100000
#define MEMORY_END   0x01000000

#define TOTAL_PAGES ((MEMORY_END - MEMORY_START) / PAGE_SIZE)
#define BITMAP_SIZE ((TOTAL_PAGES + 7) / 8)

static unsigned char bitmap[BITMAP_SIZE];

static unsigned int total_pages = TOTAL_PAGES;
static unsigned int free_pages = TOTAL_PAGES;

static void bitmap_set(unsigned int page)
{
    bitmap[page / 8] |= (1 << (page % 8));
}

static void bitmap_clear(unsigned int page)
{
    bitmap[page / 8] &= ~(1 << (page % 8));
}

static int bitmap_test(unsigned int page)
{
    return bitmap[page / 8] & (1 << (page % 8));
}

void pmm_init(void)
{
    for (unsigned int i = 0; i < BITMAP_SIZE; i++)
    {
        bitmap[i] = 0;
    }

    free_pages = total_pages;

    vga_write("Physical Memory Manager initialized!\n");
}

unsigned int pmm_alloc_page(void)
{
    for (unsigned int page = 0; page < total_pages; page++)
    {
        if (!bitmap_test(page))
        {
            bitmap_set(page);
            free_pages--;

            return MEMORY_START + (page * PAGE_SIZE);
        }
    }

    return 0;
}

void pmm_free_page(unsigned int address)
{
    if (address < MEMORY_START)
        return;

    if (address >= MEMORY_END)
        return;

    if (address % PAGE_SIZE != 0)
        return;

    unsigned int page =
        (address - MEMORY_START) / PAGE_SIZE;

    if (bitmap_test(page))
    {
        bitmap_clear(page);
        free_pages++;
    }
}

unsigned int pmm_get_total_pages(void)
{
    return total_pages;
}

unsigned int pmm_get_free_pages(void)
{
    return free_pages;
}