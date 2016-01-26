#ifndef PAGEMEM_H
#define PAGEMEM_H

#include <boot/multiboot.h>

void pagemem_init();
void *pagemem_pagedirectory_create(void *physaddr, unsigned int size);
int pagemem_pagedirectory0_add_page(void *virtaddr, void *physaddr);
void pagemem_pagedirectory_remove_page(void *virtaddr);
void *pagemem_get_physaddr(void *virtaddr);

#define release_page_frame(p_addr) \
   mem_bitmap[((unsigned int) p_addr/PAGESIZE)/8] &= ~(1 << (((unsigned int) p_addr/PAGESIZE)%8));

#define PG_PRESENT 0x0001
#define PG_WRITE   0x0002
#define PG_4MB     0x0080

/*
page table miroring explanation :
the last entry of the main page directory goes to the page directory itself,
so when you access to an address like 0xffc00000 (10 bits to 1), you access
directly to the first entry of the page directory.
In the same way, with (0xfffff000 + i) you can directly access to i'th entry
of the page table ;)
*/
#define PAGE_DIRECTORY_ENTRY_FROM_VIRTADDR(virtaddr) (unsigned int *) (0xFFFFF000 | (((unsigned int) virtaddr & 0xFFC00000) >> 20))
#define PAGE_TABLE_ENTRY_FROM_VIRTADDR(virtaddr) (unsigned int *) (0xFFC00000 | (((unsigned int) virtaddr & 0xFFFFF000) >> 10))

struct page_directory_entry
{
    unsigned char p         : 1;  /* present in memory (swap for exemple) */
    unsigned char rw        : 1;  /* 0 = r, 1 = rw                        */
    unsigned char u         : 1;  /* user bit                             */
    unsigned char pwt       : 1;  /* not used                             */
    unsigned char pcd       : 1;  /* not used                             */
    unsigned char a         : 1;  /* accessed bit                         */
    unsigned char always0   : 1;
    unsigned char size      : 1;  /* page size : 0 -> 4Ko, 1 -> 4Mo       */
    unsigned char g         : 1;  /* not used                             */
    unsigned char available : 3;  /* free of use                          */
    unsigned int address    : 20; /* page table address                   */
} __attribute__((packed));

struct page_table_entry
{
    unsigned char p         : 1;  /* present in memory (swap for exemple) */
    unsigned char rw        : 1;  /* 0 = r, 1 = rw                        */
    unsigned char u         : 1;  /* user bit                             */
    unsigned char pwt       : 1;  /* not used                             */
    unsigned char pcd       : 1;  /* not used                             */
    unsigned char a         : 1;  /* accessed bit                         */
    unsigned char d         : 1;  /* dirty flag: page has been written to */
    unsigned char always0   : 1;
    unsigned char g         : 1;  /* not used                             */
    unsigned char available : 3;  /* free of use                          */
    unsigned int address    : 20; /* page table address                   */
} __attribute__((packed));

#endif