#ifndef TASK_H
#define TASK_H

void task_load(void *physaddr, void *function, unsigned int size);

struct task
{
    unsigned int pid;

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

} __attribute__((packed));

#endif