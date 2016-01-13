#include <sys/pagemem.h>
#include <sys/asm.h>
#include <libc/mem.h>

// table of PAGE_DIRECTORY_SIZE (1024) entries
// struct page_directory_entry *page_directory = (struct page_directory_entry *) PAGE_DIRECTORY_ADDR;
// struct page_table_entry *page_table = (struct page_table_entry *) PAGE_TABLE_ADDR;

void pagemem_init()
{
    unsigned int i;
    unsigned int page_addr;
    unsigned int *page_directory =  (unsigned int *) PAGE_DIRECTORY_ADDR;
    unsigned int *page_table_0 = (unsigned int *)  PAGE_TABLE_ADDR;

    // we only use the first one for the moment
    page_directory[0] = PAGE_TABLE_ADDR;
    page_directory[0] |= 3;
    // other else are NULL
    for(i=1; i<1024; i++)
    {
        page_directory[i] = 0;
    }

    page_addr = 0;
    for(i=0; i<1024; i++, page_addr += 4096)
    {
        page_table_0[i] = page_addr;
        page_table_0[i] |= 3; // Present | Writeable
    }

    mov_cr3(PAGE_DIRECTORY_ADDR);
    cr0_enable_paging();

}