#ifndef TSSS_H
#define TSSS_H

struct tss
{
    unsigned short previous_task;
    unsigned short __previous_task_unused;
    unsigned int esp0;
    unsigned short ss0;
    unsigned short __ss0_unused;
    unsigned int esp1;
    unsigned short ss1;
    unsigned short __ss1_unused;
    unsigned int esp2;
    unsigned short ss2;
    unsigned short __ss2_unused;
    unsigned int cr3;
    unsigned int eip;
    unsigned int eflags;
    unsigned int eax;
    unsigned int ecx;
    unsigned int edx;
    unsigned int ebx;
    unsigned int esp;
    unsigned int ebp;
    unsigned int esi;
    unsigned int edi;
    unsigned short es;
    unsigned short __es_unused;
    unsigned short cs;
    unsigned short __cs_unused;
    unsigned short ss;
    unsigned short __ss_unused;
    unsigned short ds;
    unsigned short __ds_unused;
    unsigned short fs;
    unsigned short __fs_unused;
    unsigned short gs;
    unsigned short __gs_unused;
    unsigned short ldt_selector;
    unsigned short __ldt_sel_unused;
    unsigned short debug_flag;
    unsigned short io_map;
} __attribute__((packed));

struct tss *tss_get(void);
void tss_update(unsigned short ss0, unsigned int esp0);

#endif