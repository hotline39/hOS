#include "heap.h"
#include "vga.h"

#define HEAP_START 0x00300000
#define HEAP_END   0x00400000

static unsigned int heap_current;

void heap_init(void)
{
    heap_current = HEAP_START;
    vga_write("Kernel Heap initialized!\n");
}

void *kmalloc(unsigned int size)
{
    unsigned int address;

    if (size == 0)
        return 0;

    if (heap_current + size > HEAP_END)
        return 0;

    address = heap_current;
    heap_current += size;

    return (void *)address;
}

void kfree(void *ptr)
{
    (void)ptr;
}