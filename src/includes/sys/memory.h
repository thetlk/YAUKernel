#ifndef MEMORY_H
#define MEMORY_H

#include <boot/multiboot.h>

/*
Virtual memory organization :
    4Go ----
        user space
    1Go
        kernel space
    0x0 ----

Kernel space organization :
    1Go - 0x40000000
        heap
    256Mo - 0x10000000
    16Mo - 0x1000000
        heap de page
    8Mo - 0x800000
        kernel code && data
    8291 - 0x2000
        kernel page directory
    4096 - 0x1000
    0x0

Physical memory organization :
    identity mapping from 0x0 to 0x800000
    from 0x400000 to 0x800000 page tables of kernel

*/

#define KERNEL_MAX_ADDR 0x800000 // 8Mo
#define KERNEL_PAGE_DIRECTORY_ADDR 0x1000
#define KERNEL_PAGE_TABLE_ADDR 0x400000
#define KERNEL_HEAP 0x10000000
#define KERNEL_HEAP_MAX 0x40000000
#define USER_SPACE_BASE_ADDR 0x40000000

#define PAGE_SIZE 4096
#define RAM_MAXPAGE 0x100000 // 0x100000*4096 bytes = 4 294 967 296 bytes --> 4Go

#define ALIGN_PAGE_INF(addr) ((unsigned int)(addr) & ~(PAGE_SIZE - 1))
#define ALIGN_PAGE_SUP(addr) (((unsigned int)(addr) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define GET_PAGE_INF(addr) (ALIGN_PAGE_INF(addr) / PAGE_SIZE)
#define GET_PAGE_SUP(addr) (ALIGN_PAGE_SUP(addr) / PAGE_SIZE)

void memory_set_page_used(unsigned int page);
void *memory_get_unused_page();
void memory_init(struct multiboot_info *mbi);

#endif