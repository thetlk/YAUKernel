#include <sys/kmalloc.h>
#include <sys/memory.h>
#include <sys/pagemem.h>
#include <driver/video.h>

void *kernel_heap;

void kmalloc_init()
{
    kernel_heap = (void *) KERNEL_HEAP;
    kmalloc_ksbrk(1);
}

void *kmalloc_ksbrk(unsigned int n)
{
    unsigned int i;
    void *page;
    struct kmalloc_chunk *chunk;

    if(kernel_heap + (n * PAGE_SIZE) > (void *) KERNEL_HEAP_MAX)
    {
        video_printf("kmalloc_ksbrk(): no virtual memory left for the kernel heap\n");
        return (void *) -1;
    }

    chunk = (struct kmalloc_chunk *) kernel_heap;
    for(i=0; i<n; i++)
    {
        page = memory_get_unused_page();
        if((int) page == -1)
        {
            video_printf("kmalloc_ksbrk(): no physical memory left for the kernel heap\n");
            return (void *) -1;
        }

        pagemem_pd0_add_page(kernel_heap, page);

        kernel_heap += PAGE_SIZE;
    }

    chunk->size = PAGE_SIZE * n;
    chunk->used = 0;

    return 0;
}
