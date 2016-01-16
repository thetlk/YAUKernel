#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <sys/task.h>

struct pushed_registers
{
    unsigned int gs;
    unsigned int fs;
    unsigned int es;
    unsigned int ds;
    unsigned int edi;
    unsigned int esi;
    unsigned int ebp;
    unsigned int esp_naze;
    unsigned int ebx;
    unsigned int edx;
    unsigned int ecx;
    unsigned int eax;
    // because of iret
    unsigned int eip;
    unsigned int cs;
    unsigned int eflags;
    unsigned int esp;
    unsigned int ss;
};

void scheduler_add_task(struct task t);
void schedule(struct pushed_registers *regs);

#endif