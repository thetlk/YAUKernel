#include <sys/kmalloc.h>
#include <sys/memory.h>
#include <sys/pagemem.h>
#include <driver/video.h>

void *kernel_heap;

void *kmalloc_ksbrk(unsigned int n)
{
    unsigned int i;
    void *page;
    struct kmalloc_chunk *chunk;

    if(((unsigned int) kernel_heap + (n * PAGE_SIZE)) > KERNEL_HEAP_MAX)
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

        pagemem_pagedirectory0_add_page(kernel_heap, page);

        kernel_heap = (void*) (((unsigned int) kernel_heap) + PAGE_SIZE);
    }

    chunk->size = PAGE_SIZE * n;
    chunk->used = 0;

    return 0;
}

void kmalloc_init()
{
    kernel_heap = (void *) KERNEL_HEAP;
    kmalloc_ksbrk(1);
}

void *kmalloc(unsigned int size)
{
    unsigned int alloc_size;
    struct kmalloc_chunk *chunk;
    struct kmalloc_chunk *split;

    alloc_size = sizeof(struct kmalloc_chunk) + size;
    if(alloc_size < 16)
    {
        alloc_size = 16;
    }

    chunk = (struct kmalloc_chunk *) KERNEL_HEAP;
    while(chunk->used == 1 || chunk->size < alloc_size)
    {

        if(chunk->size == 0)
        {
            video_print("kmalloc(): chunk seems to be corrupted (size = 0)\n");
            asm volatile("hlt");
        }

        chunk = (struct kmalloc_chunk *) ((char *) chunk + chunk->size);

        if(chunk == kernel_heap)
        {
            if(kmalloc_ksbrk((alloc_size / PAGE_SIZE) + 1) == (void *) -1)
            {
                video_print("kmalloc(): no memory left :'(\n");
                asm volatile("hlt");
            }
        }

        if(chunk > (struct kmalloc_chunk *) kernel_heap)
        {
            video_printf("chunk at %p but heap limit is at %p !\n", chunk, kernel_heap);
            asm volatile("hlt");
        }

    }

    // only split chunk when chunk->size != alloc_size
    if(chunk->size != alloc_size)
    {
        split = (struct kmalloc_chunk *) ((char *) chunk + alloc_size);
        split->size = chunk->size - alloc_size;
        split->used = 0;
    }

    chunk->size = alloc_size;
    chunk->used = 1;

    return (char *) chunk + sizeof(struct kmalloc_chunk);
}

void kfree(void *addr)
{
    struct kmalloc_chunk *chunk;
    struct kmalloc_chunk *next;

    chunk = (struct kmalloc_chunk *) (((unsigned int) addr) - sizeof(struct kmalloc_chunk));
    chunk->used = 0;

    // merge this chunk with next free chunks
    while((next = (struct kmalloc_chunk *) ((char *) chunk + chunk->size))
        && next < (struct kmalloc_chunk *) kernel_heap
        && next->used == 0)
    {
        chunk->size += next->size;
    }
}
