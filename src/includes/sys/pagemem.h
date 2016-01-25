#ifndef PAGEMEM_H
#define PAGEMEM_H

#include <boot/multiboot.h>

void pagemem_init(struct multiboot_info *mbi);
void *pagemem_pagedirectory_create(void *physaddr, unsigned int size);
int pagemem_pd0_add_page(void *virtaddr, void *physaddr);

#define release_page_frame(p_addr) \
   mem_bitmap[((unsigned int) p_addr/PAGESIZE)/8] &= ~(1 << (((unsigned int) p_addr/PAGESIZE)%8));

#define PG_PRESENT 0x0001
#define PG_WRITE   0x0002
#define PG_4MB     0x0080

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