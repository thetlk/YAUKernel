#include <sys/memory.h>
#include <boot/multiboot.h>
#include <driver/video.h>

unsigned char mem_bitmap[RAM_MAXPAGE / 8];

void memory_set_page_used(unsigned int page)
{
    mem_bitmap[page/8] |= (1 << (page%8));
}

void *memory_get_unused_page()
{
    unsigned int byte;
    unsigned int bit;
    unsigned int page;
    void *physaddr;

    for(byte = 0; byte<RAM_MAXPAGE/8; byte++)
    {
        if(mem_bitmap[byte] != 0xFF)
        {
            for(bit = 0; bit<8; bit++)
            {
                if((mem_bitmap[byte] & (1<<bit)) == 0)
                {
                    page = 8* byte + bit;
                    physaddr = (void *) (page + PAGE_SIZE);
                    memory_set_page_used(page);
                    return physaddr;
                }
            }
        }
    }

    return (void *) -1;
}

void memory_init(struct multiboot_info *mbi)
{
    unsigned int i;
    unsigned int max_page = GET_PAGE_INF(mbi->mem_upper * 1024);
    
    // empty bitmap
    for(i=0; i<max_page/8; i++)
    {
        mem_bitmap[i] = 0;
    }

    // set used page that don't exists
    for(i=max_page/8; i<RAM_MAXPAGE/8; i++)
    {
        mem_bitmap[i] = 0xFF;
    }

    // kernel pages
    for(i=GET_PAGE_INF(0x0); i<GET_PAGE_INF(KERNEL_MAX_ADDR); i++)
    {
        memory_set_page_used(i);
    }

    // set page used for each reserved range
    if((mbi->flags) & (1 << (FLAG_MMAP)))
    {
        struct multiboot_memory_map *mmap = (struct multiboot_memory_map *) mbi->mmap_addr;

        while((unsigned int) mmap < mbi->mmap_addr + mbi->mmap_length)
        {
            if(mmap->type != 1)
            {
                for(i=GET_PAGE_INF(mmap->base); i<GET_PAGE_INF(mmap->base + mmap->length); i++)
                {
                    memory_set_page_used(i);
                }
            }
            mmap = (struct multiboot_memory_map *) ((unsigned int) mmap + mmap->size + sizeof(mmap->size));
        }
    }
}
