#include <sys/pagemem.h>
#include <sys/asm.h>
#include <libc/string.h>
#include <boot/multiboot.h>
#include <driver/video.h>

unsigned int *pd0 = (unsigned int *) KERNEL_PAGE_DIRECTORY_ADDR; /* kernel page directory */
unsigned int *pg0 = (unsigned int *) KERNEL_PAGE_0;
unsigned int *pg1 = (unsigned int *) KERNEL_PAGE_1;
unsigned char mem_bitmap[RAM_MAXPAGE / 8];

void set_page_frame_used(unsigned int page)
{
    mem_bitmap[page/8] |= (1 << (page%8));
}

// return PHYS addr
void *pagemem_get_page_frame()
{
    unsigned int byte = 0;
    unsigned int bit = 0;
    unsigned int page = 0;
    void *physaddr = 0;

    for(byte = 0; byte < RAM_MAXPAGE/8; byte++)
    {
        if(mem_bitmap[byte] != 0xFF)
        {
            for(bit = 0; bit < 8; bit++)
            {
                if((mem_bitmap[byte] & (1<<bit)) == 0)
                {
                    page = 8 * byte + bit;
                    physaddr = (void*) (page * PAGESIZE);
                    set_page_frame_used(page);
                    return physaddr;
                }
            }
        }
    }

    return 0;
}

void pagemem_init(struct multiboot_info *mbi)
{
    unsigned int i;

    pd0[0] = (unsigned int) pg0 | PG_PRESENT | PG_WRITE | PG_4MB;
    pd0[1] = (unsigned int) pg1 | PG_PRESENT | PG_WRITE | PG_4MB;
    for(i=2; i<1023; i++)
    {
        pd0[i] = (unsigned int) pg1 + PAGESIZE * i;
        pd0[i] |= PG_PRESENT | PG_WRITE;
        // pd0[i] = 0;
    }
    pd0[1023] = (unsigned int) pd0 | PG_PRESENT | PG_WRITE;

    mov_cr3(pd0);
    cr4_set_pse_flag();
    cr0_enable_paging();

}

void *pagemem_pagedirectory_create(void *physaddr, unsigned int size)
{
    unsigned int page_base = PAGE((unsigned int) physaddr);
    unsigned int pages;
    unsigned int i;
    unsigned int j;
    unsigned int *page_directory;
    unsigned int *page_table;

    if(size % PAGESIZE != 0)
    {
        pages = size / PAGESIZE + 1;
    } else {
        pages = size / PAGESIZE;
    }

    for(i=0; i<pages; i++)
    {
        set_page_frame_used(page_base + i);
    }

    // setup kernel
    page_directory = (unsigned int *) pagemem_get_page_frame();
    page_directory[0] = pd0[0];
    page_directory[0] |= 3;
    for(i=1; i<1024; i++)
    {
        page_directory[i] = 0;
    }

    // userspace
    for(i=0; pages; i++)
    {
        page_table = (unsigned int *) pagemem_get_page_frame();
        page_directory[(USER_OFFSET + i * PAGESIZE * 1024) >> 22] = (unsigned int) page_table;
        page_directory[(USER_OFFSET + i * PAGESIZE * 1024) >> 22] |= 7;

        for(j=0; j<1024 && pages; j++, pages--)
        {
            page_table[j] = ((unsigned int) physaddr + i * PAGESIZE * 1024 + j * PAGESIZE);
            page_table[j] |= 7;
        }
    }

    return page_directory;
}
