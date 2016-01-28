#ifndef KMALLOC_H
#define KMALLOC_H

struct kmalloc_chunk
{
    unsigned int size : 31;
    unsigned int used : 1;
} __attribute__((packed));

void kmalloc_init();
void *kmalloc(unsigned int size);
void kfree(void *addr);
void *kmalloc_ksbrk(unsigned int n);

#endif