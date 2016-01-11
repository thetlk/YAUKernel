#ifndef PAGEMEM_H
#define PAGEMEM_H

void pagemem_init();

#define PAGE_DIRECTORY_SIZE 1024
#define PAGE_DIRECTORY_ADDR 0x20000
#define PAGE_TABLE_SIZE 1024
#define PAGE_TABLE_ADDR 0x21000

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