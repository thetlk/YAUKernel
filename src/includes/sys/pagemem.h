#ifndef PAGEMEM_H
#define PAGEMEM_H

void pagemem_init();
void pd_create_task1();

#define PAGESIZE 4096
#define RAM_MAXPAGE 0x10000
#define VADDR_PD_OFFSET(addr)   ((addr) & 0xFFC00000) >> 22
#define VADDR_PT_OFFSET(addr)   ((addr) & 0x003FF000) >> 12
#define VADDR_PG_OFFSET(addr)   (addr) & 0x00000FFF
#define PAGE(addr)              (addr) >> 12

 #define USER_OFFSET 0x40000000

#define set_page_frame_used(page) \
    mem_bitmap[((unsigned int) page)/8] |= (1 << (((unsigned int) page)%8));

 #define release_page_frame(p_addr) \
    mem_bitmap[((unsigned int) p_addr/PAGESIZE)/8] &= ~(1 << (((unsigned int) p_addr/PAGESIZE)%8));

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