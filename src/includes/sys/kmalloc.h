#ifndef KMALLOC_H
#define KMALLOC_H

struct kmalloc_chunk
{
    unsigned int size;
    unsigned char used;
} __attribute__((packed));

void kmalloc_init(void);
void *kmalloc(unsigned int size);
void kfree(void *addr);
void *kmalloc_ksbrk(unsigned int n);

#endif