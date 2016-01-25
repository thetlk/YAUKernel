#include <sys/pagemem.h>
#include <sys/asm.h>
#include <sys/memory.h>
#include <libc/string.h>
#include <boot/multiboot.h>
#include <driver/video.h>

unsigned int *pd0 = (unsigned int *) KERNEL_PAGE_DIRECTORY_ADDR; /* kernel page directory */

void pagemem_init()
{
    unsigned int i;

    // identity mapping from 0x0 to 0x800000
    pd0[0] = 0x000000 | PG_PRESENT | PG_WRITE | PG_4MB;
    pd0[1] = 0x400000 | PG_PRESENT | PG_WRITE | PG_4MB;
    for(i=2; i<1023; i++)
    {
        pd0[i] = (unsigned int) 0x400000 + PAGE_SIZE * i;
        pd0[i] |= PG_PRESENT | PG_WRITE;
        // pd0[i] = 0;
    }

    // page directory miroring
    pd0[1023] = (unsigned int) pd0 | PG_PRESENT | PG_WRITE;

    mov_cr3(pd0);
    cr4_set_pse_flag();
    cr0_enable_paging();
    // asm volatile("xchg %%bx, %%bx ;" ::: );

}

int pagemem_pd0_add_page(void *virtaddr, void *physaddr)
{
    unsigned int *page_directory_entry;
    unsigned int *page_table_entry;

    if((unsigned int) virtaddr > USER_SPACE_BASE_ADDR)
    {
        video_printf("pagemem_pd0_add_page() : %p not in kernel space\n", virtaddr);
        return -1;
    }

    /*
    page table miroring explanation :
    the last entry of the main page directory goes to the page directory itself,
    so when you access to an address like 0xffc00000 (10 bits to 1), you access
    directly to the first entry of the page directory.
    In the same way, with (0xfffff000 + i) you can directly access to i'th entry
    of the page table ;)
    */
    page_directory_entry = (unsigned int *) (0xFFFFF000 | (((unsigned int) virtaddr & 0xFFC00000) >> 20));
    if((*page_directory_entry & PG_PRESENT) == 0)
    {
        video_printf("pagemem_pd0_add_page(): kernel page table not found for virtaddr %p\n", virtaddr);
        asm volatile("hlt");
    }

    page_table_entry = (unsigned int *) (0xFFC00000 | (((unsigned int) virtaddr & 0xFFFFF000) >> 10));
    *page_table_entry = (unsigned int) physaddr | (PG_PRESENT | PG_WRITE);

    return 0;
}

void *pagemem_pagedirectory_create(void *physaddr, unsigned int size)
{
    unsigned int page_base = GET_PAGE_INF((unsigned int) physaddr);
    unsigned int pages;
    unsigned int i;
    unsigned int j;
    unsigned int *page_directory;
    unsigned int *page_table;

    if(size % PAGE_SIZE != 0)
    {
        pages = size / PAGE_SIZE + 1;
    } else {
        pages = size / PAGE_SIZE;
    }

    for(i=0; i<pages; i++)
    {
        memory_set_page_used(page_base + i);
    }

    // setup kernel
    page_directory = (unsigned int *) memory_get_unused_page();
    page_directory[0] = pd0[0];
    page_directory[0] |= 3;
    for(i=1; i<1024; i++)
    {
        page_directory[i] = 0;
    }

    // userspace
    for(i=0; pages; i++)
    {
        page_table = (unsigned int *) memory_get_unused_page();
        page_directory[(USER_SPACE_BASE_ADDR + i * PAGE_SIZE * 1024) >> 22] = (unsigned int) page_table;
        page_directory[(USER_SPACE_BASE_ADDR + i * PAGE_SIZE * 1024) >> 22] |= 7;

        for(j=0; j<1024 && pages; j++, pages--)
        {
            page_table[j] = ((unsigned int) physaddr + i * PAGE_SIZE * 1024 + j * PAGE_SIZE);
            page_table[j] |= 7;
        }
    }

    return page_directory;
}
