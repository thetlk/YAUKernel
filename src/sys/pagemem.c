#include <sys/pagemem.h>
#include <sys/asm.h>
#include <libc/mem.h>
#include <driver/video.h>

unsigned int *pd0; /* kernel page directory */
unsigned int *pt0; /* kernel page table */
unsigned char mem_bitmap[RAM_MAXPAGE / 8];

void set_page_frame_used(unsigned int page)
{
    mem_bitmap[page/8] |= (1 << (page%8));
}

// return PHYS addr
void *get_page_frame()
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

void pagemem_init()
{
    unsigned int i;
    unsigned int page_addr;

    // empty bitmap
    for(i=0; i<RAM_MAXPAGE/8; i++)
    {
        mem_bitmap[i] = 0;
    }

    // kernel pages
    for(i=PAGE(0x0); i<PAGE(0x20000); i++)
    {
        set_page_frame_used(i);
    }

    // hardware pages
    for(i=PAGE(0xA0000); i<PAGE(0x100000); i++)
    {
        set_page_frame_used(i);
    }

    pd0 = get_page_frame();
    pt0 = get_page_frame();

    pd0[0] = (unsigned int) pt0;
    pd0[0] |= 3; // present | writable
    for(i=1; i<1024; i++)
    {
        pd0[i] = 0;
    }

    page_addr = 0;
    for(i=0; i<1024; i++, page_addr += 4096)
    {
        pt0[i] = page_addr;
        pt0[i] |= 3; // Present | Writeable
    }

    mov_cr3(pd0);
    cr0_enable_paging();

}

void *page_directory_create(void *physaddr, unsigned int size)
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
    page_directory = (unsigned int *) get_page_frame();
    page_directory[0] = pd0[0];
    page_directory[0] |= 3;
    for(i=1; i<1024; i++)
    {
        page_directory[i] = 0;
    }

    // userspace
    for(i=0; pages; i++)
    {
        page_table = (unsigned int *) get_page_frame();
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
