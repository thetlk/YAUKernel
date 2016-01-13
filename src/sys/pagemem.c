#include <sys/pagemem.h>
#include <sys/asm.h>
#include <libc/mem.h>

// table of PAGE_DIRECTORY_SIZE (1024) entries
struct page_directory_entry *page_directory = (struct page_directory_entry *) PAGE_DIRECTORY_ADDR;
struct page_table_entry *page_table = (struct page_table_entry *) PAGE_TABLE_ADDR;

void pagemem_init()
{
    unsigned int i;
    unsigned int page_addr;

    // memset((unsigned char *) page_directory, 0, sizeof(struct page_directory_entry) * PAGE_DIRECTORY_SIZE);
    // page_directory[0].p = 1;
    // page_directory[0].rw = 1;
    // page_directory[0].address = PAGE_TABLE_ADDR;

    // memset((unsigned char *) page_table, 0, sizeof(struct page_table_entry) * PAGE_TABLE_SIZE);
    // page_addr = 0;
    // for(i=0; i<PAGE_TABLE_SIZE; i++)
    // {
    //     page_table[i].address = page_addr;
    //     page_table[i].p = 1;
    //     page_table[i].rw = 1;
    //     page_addr += 4096;
    // }

    unsigned int *pd0 =  (unsigned int *) PAGE_DIRECTORY_ADDR;
    unsigned int *pt0 = (unsigned int *)  PAGE_TABLE_ADDR;

    pd0[0] = PAGE_TABLE_ADDR;
    pd0[0] |= 3;
    for (i = 1; i < 1024; i++){
        pd0[i] = 0;
    }

        page_addr = 0;
        for (i = 0; i < 1024; i++) {
                pt0[i] = page_addr;
                pt0[i] |= 3;
                page_addr += 4096;
        }


    mov_cr3(PAGE_DIRECTORY_ADDR);
    cr0_enable_paging();

}