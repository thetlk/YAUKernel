#include <sys/pagemem.h>
#include <sys/asm.h>
#include <libc/mem.h>
#include <driver/video.h>

unsigned int *pd0; /* kernel page directory */
unsigned int *pt0; /* kernel page table */
unsigned char mem_bitmap[RAM_MAXPAGE / 8];

// return PHYS addr
void *get_page_frame()
{
    unsigned int byte;
    unsigned int bit;
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
                    set_page_frame_used(page);
                    physaddr = (void*) (page * PAGESIZE);
                    break;
                }
            }
        }
    }

    video_print("return page = 0x");
    video_print_number((unsigned int) page, 16);
    video_putchar('\n');

    return physaddr;
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

    video_print("pd0 = ");
    video_print_number((unsigned int) pd0, 16);
    video_putchar('\n');

    video_print("pt0 = ");
    video_print_number((unsigned int) pt0, 16);
    video_putchar('\n');

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

    // mov_cr3(pd0);
    // cr0_enable_paging();

}

unsigned int *pd_create_task1()
{
    unsigned int *pd;
    unsigned int *pt;
    unsigned int i;

    pd = get_page_frame();
    for(i=0; i<1024; i++)
    {
        pd[i] = 0;
    }

    pt = get_page_frame();
    for(i=0; i<1024; i++)
    {
        pt[i] = 0;
    }

    pd[0] = pd0[0];
    pd[0] |= 3;

    pd[USER_OFFSET >> 22] = (unsigned int) pt;
    pd[USER_OFFSET >> 22] |= 7;

    pt[0] = 0x100000;
    pt[0] |= 7;

    return pd;
}
