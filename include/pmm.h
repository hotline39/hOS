#ifndef PMM_H
#define PMM_H

#define PAGE_SIZE 4096

void pmm_init(void);

unsigned int pmm_alloc_page(void);
void pmm_free_page(unsigned int address);

unsigned int pmm_get_total_pages(void);
unsigned int pmm_get_free_pages(void);

#endif