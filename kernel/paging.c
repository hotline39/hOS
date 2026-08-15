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

    /*
     * Page Directory 초기화
     */
    for (i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        page_directory[i] = 0;
    }

    /*
     * 첫 번째 Page Table 생성
     *
     * 0x00000000 ~ 0x003FFFFF
     * 를 identity mapping 한다.
     */
    for (i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        first_page_table[i] =
            (i * PAGE_SIZE) |
            PAGE_PRESENT |
            PAGE_WRITABLE;
    }

    /*
     * Page Directory의 첫 번째 항목이
     * 첫 번째 Page Table을 가리키도록 한다.
     */
    page_directory[0] =
        ((unsigned int)first_page_table) |
        PAGE_PRESENT |
        PAGE_WRITABLE;

    /*
     * CR3에 Page Directory 주소 등록
     */
    load_page_directory((unsigned int)page_directory);

    /*
     * CR0.PG = 1
     *
     * Paging 활성화
     */
    enable_paging();

    vga_write("Paging enabled!\n");
}