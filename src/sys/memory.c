#include <sys/memory.h>
#include <sys/kmalloc.h>
#include <sys/pagemem.h>
#include <boot/multiboot.h>
#include <driver/video.h>

unsigned char mem_bitmap[RAM_MAXPAGE / 8];
unsigned char page_heap_bitmap[KERNEL_PAGE_HEAP_NUM / 8];

void memory_set_page_used(unsigned int page)
{
    mem_bitmap[page/8] |= (1 << (page%8));
}

void memory_set_page_unused(unsigned int page)
{
    mem_bitmap[page/8] &= ~(1 << ((page%8)));
    video_printf("page is now uunsed : %p\n", page);
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
                    page = 8 * byte + bit;
                    physaddr = (void *) (page * PAGE_SIZE);
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

    // initialize page heap for the kernel
    for(i=0; i<KERNEL_PAGE_HEAP_NUM/8; i++)
    {
        page_heap_bitmap[i] = 0;
    }
}

void __memory_set_page_heap_used(unsigned int page)
{
    video_printf("used : %d\n", page);
    page_heap_bitmap[page/8] |= (1 << (page%8));
}

void __memory_set_page_heap_unused(unsigned int page)
{
    video_printf("uunsed : %d\n", page);
    page_heap_bitmap[page/8] &= ~(1 << ((page%8)));
}

void *__memory_get_page_heap_virtaddr()
{
    unsigned int byte;
    unsigned int bit;
    unsigned int page;
    void *virtaddr;

    for(byte=0; byte<KERNEL_PAGE_HEAP_NUM/8; byte++)
    {
        if(page_heap_bitmap[byte] != 0xFF)
        {
            for(bit=0; bit<8; bit++)
            {
                if((page_heap_bitmap[byte] & (1<<bit)) == 0)
                {
                    page = 8 * byte + bit;
                    __memory_set_page_heap_used(page);
                    virtaddr = (void *) (page * PAGE_SIZE) + KERNEL_PAGE_HEAP;
                    return virtaddr;
                }
            }
        }
    }

    return (void *) -1;
}

struct page *memory_get_page_heap()
{
    struct page *ret;
    void *physaddr;
    void *virtaddr;

    physaddr = memory_get_unused_page();
    if(physaddr == (void *) -1)
    {
        video_printf("memory_get_page_heap(): no physical page available.\n");
        asm volatile("hlt");
    }

    virtaddr = __memory_get_page_heap_virtaddr();
    if(virtaddr == (void *) -1)
    {
        video_printf("memory_get_page_heap(): no virtual page available.\n");
        asm volatile("hlt");
    }

    if(pagemem_pagedirectory0_add_page(virtaddr, physaddr) == -1)
    {
        video_printf("memory_get_page_heap(): unable to map :'(\n");
        asm volatile("hlt");
    }

    ret = (struct page *) kmalloc(sizeof(struct page));
    ret->virtaddr = virtaddr;
    ret->physaddr = physaddr;

    return ret;
}

int memory_release_page_heap(void *virtaddr)
{
    void *physaddr;

    physaddr = pagemem_get_physaddr(virtaddr);
    if(physaddr == 0)
    {
        video_printf("memory_release_page_heap(): no physaddr associated to %p\n", virtaddr);
        return 1;
    }

    memory_set_page_unused(GET_PAGE_INF(physaddr));
    pagemem_pagedirectory_remove_page(virtaddr);

    __memory_set_page_heap_unused(((unsigned int) virtaddr - KERNEL_PAGE_HEAP) / PAGE_SIZE);

    return 0;
}
