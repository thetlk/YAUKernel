#ifndef MEMORY_H
#define MEMORY_H

#include <boot/multiboot.h>

#define KERNEL_MAX_ADDR 0x800000 // 8Mo

#define PAGE_SIZE 4096
#define RAM_MAXPAGE 0x100000 // 0x100000*4096 bytes = 4 294 967 296 bytes --> 4Go

#define ALIGN_PAGE_INF(addr) ((unsigned int)(addr) & ~(PAGE_SIZE - 1))
#define ALIGN_PAGE_SUP(addr) (((unsigned int)(addr) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define GET_PAGE_INF(addr) (ALIGN_PAGE_INF(addr) / PAGE_SIZE)

void memory_init(struct multiboot_info *mbi);

#endif