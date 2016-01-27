#include <sys/pagemem.h>
#include <sys/asm.h>
#include <sys/memory.h>
#include <sys/kmalloc.h>
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

int pagemem_pagedirectory0_add_page(void *virtaddr, void *physaddr)
{
    unsigned int *page_directory_entry;
    unsigned int *page_table_entry;

    if((unsigned int) virtaddr > USER_SPACE_BASE_ADDR)
    {
        video_printf("pagemem_pagedirectory0_add_page() : %p not in kernel space\n", virtaddr);
        return -1;
    }

    page_directory_entry = PAGE_DIRECTORY_ENTRY_FROM_VIRTADDR(virtaddr);
    if((*page_directory_entry & PG_PRESENT) == 0)
    {
        video_printf("pagemem_pagedirectory0_add_page(): kernel page table not found for virtaddr %p\n", virtaddr);
        asm volatile("hlt");
    }

    page_table_entry = PAGE_TABLE_ENTRY_FROM_VIRTADDR(virtaddr);
    *page_table_entry = (unsigned int) physaddr | (PG_PRESENT | PG_WRITE);

    return 0;
}

void pagemem_pagedirectory_remove_page(void *virtaddr)
{
    unsigned int *page_table_entry;

    if(pagemem_get_physaddr(virtaddr)) // addr is mapped <=>
    {
        page_table_entry = PAGE_TABLE_ENTRY_FROM_VIRTADDR(virtaddr);
        *page_table_entry = *page_table_entry & (~PG_PRESENT);
        invlpg(virtaddr);
    }
}

struct page_directory * pagemem_pagedirectory_create()
{
    struct page_directory *page_directory;
    unsigned int *page_directory_addr;
    unsigned int i;

    page_directory = (struct page_directory *) kmalloc(sizeof(struct page_directory));
    page_directory->base = memory_get_page_heap();

    page_directory_addr = (unsigned int *) page_directory->base->virtaddr;
    // first 1Go --> kernel space
    for(i=0; i<256; i++)
    {
        page_directory_addr[i] = pd0[i];
    }

    // user space
    for(i=256; i<1023; i++)
    {
        page_directory_addr[i] = 0;
    }

    // page directory miroring
    page_directory_addr[1023] = ((unsigned int) page_directory->base->physaddr) | PG_PRESENT | PG_WRITE;

    page_directory->pages = 0;

    return page_directory;
}

void pagemem_pagedirectory_add_page(struct page_directory *page_directory, void *virtaddr, void *physaddr)
{
    unsigned int *page_directory_entry;
    unsigned int *page_table_entry;
    unsigned int *page_table;
    unsigned int i;
    struct page *new_page;
    struct page_list *new_elem;

    page_directory_entry = PAGE_DIRECTORY_ENTRY_FROM_VIRTADDR(virtaddr);

    // create page directory entry if not exists
    if((*page_directory_entry & PG_PRESENT) == 0)
    {
        new_page = memory_get_page_heap();
        page_table = (unsigned int *) new_page->virtaddr;

        for(i=0; i<1024; i++)
        {
            page_table[i] = 0;
        }

        *page_directory_entry = ((unsigned int) new_page->physaddr) | PG_PRESENT | PG_WRITE | PG_USER;

        if(page_directory->pages == 0) // first entry
        {
            new_elem = kmalloc(sizeof(struct page_list));
            new_elem->page = new_page;
            new_elem->next = 0;
            new_elem->prev = 0;
            page_directory->pages = new_elem;
        } else {
            new_elem = kmalloc(sizeof(struct page_list));
            new_elem->page = new_page;
            new_elem->next = page_directory->pages;
            new_elem->prev = 0;
            page_directory->pages->prev = new_elem;
            page_directory->pages = new_elem;
        }

    }

    // add page table entry
    page_table_entry = PAGE_TABLE_ENTRY_FROM_VIRTADDR(virtaddr);
    *page_table_entry = ((unsigned int) physaddr) | PG_PRESENT | PG_WRITE | PG_USER;
}

struct page_list *pagemem_pagedirectory_map(struct page_directory *page_directory, void *virtaddr, unsigned int size)
{
    struct page_list *page_list;
    void *physaddr;
    unsigned int i;

    page_list = kmalloc(sizeof(struct page_list));
    page_list->page = 0;
    page_list->next = 0;
    page_list->prev = 0;

    for(i=((unsigned int) virtaddr); i < ((unsigned int) virtaddr) + size; i += PAGE_SIZE)
    {
        physaddr = memory_get_unused_page();
        pagemem_pagedirectory_add_page(page_directory, (void*) i, physaddr);

        page_list->page = kmalloc(sizeof(struct page));
        page_list->page->virtaddr = (void*) i;
        page_list->page->physaddr = physaddr;

        page_list->next = kmalloc(sizeof(struct page_list));
        page_list->next->page = 0;
        page_list->next->next = 0;
        page_list->next->prev = page_list;

        page_list = page_list->next;
    }

    return page_list;
}

void *pagemem_get_physaddr(void *virtaddr)
{
    unsigned int *page_directory_entry;
    unsigned int *page_table_entry;

    page_directory_entry = PAGE_DIRECTORY_ENTRY_FROM_VIRTADDR(virtaddr);
    if((*page_directory_entry & PG_PRESENT) == 1)
    {
        page_table_entry = PAGE_TABLE_ENTRY_FROM_VIRTADDR(virtaddr);
        if((*page_table_entry & PG_PRESENT) == 1)
        {
            return (void*) ((*page_table_entry & 0xFFFFF000) + ((unsigned int) virtaddr & 0x00000FFF));
        }
    }

    return 0;
}
