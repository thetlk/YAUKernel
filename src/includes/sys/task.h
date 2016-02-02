#ifndef TASK_H
#define TASK_H

#include <sys/pagemem.h>

struct task
{
    unsigned int pid;
    struct page_directory *page_directory;
    struct page_list *page_list;

    struct
    {
        unsigned int eax;
        unsigned int ebx;
        unsigned int ecx;
        unsigned int edx;
        unsigned int esi;
        unsigned int edi;
        unsigned int esp;
        unsigned int ebp;
        unsigned int eip;
        unsigned int eflags;
        unsigned int cs;
        unsigned int ss;
        unsigned int ds;
        unsigned int es;
        unsigned int fs;
        unsigned int gs;
        unsigned int cr3;
    } regs __attribute__((packed));

    struct
    {
        unsigned int esp0;
        unsigned short ss0;
    } kstack __attribute__((packed));

} __attribute__((packed));

struct task *task_load(void *function, unsigned int size);
void task_load_and_schedule(void *function, unsigned int size);

#endif