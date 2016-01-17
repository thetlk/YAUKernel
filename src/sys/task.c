#include <sys/task.h>
#include <sys/pagemem.h>
#include <sys/scheduler.h>
#include <libc/mem.h>
#include <driver/video.h>

void task_load(void *physaddr, void *function, unsigned int size)
{
    void *page_directory;
    void *kernel_stack;
    struct task t;

    // copy code at physical address
    memcpy(physaddr, function, size);

    page_directory = pagemem_pagedirectory_create(physaddr, size);
    kernel_stack = pagemem_get_page_frame();

    t.kstack.ss0 = 0x18;
    t.kstack.esp0 = (unsigned int) kernel_stack + PAGESIZE;
    t.regs.ss = 0x33;
    t.regs.esp = 0x40001000;
    t.regs.cs = 0x23;
    t.regs.eip = 0x40000000;
    t.regs.ds = 0x2B;
    t.regs.fs = 0x2B;
    t.regs.es = 0x2B;
    t.regs.gs = 0x2B;
    t.regs.cr3 = (unsigned int) page_directory;
    t.regs.eflags = 0x0;
    t.regs.eax = 0;
    t.regs.ebx = 0;
    t.regs.ecx = 0;
    t.regs.edx = 0;
    t.regs.esi = 0;
    t.regs.edi = 0;

    scheduler_add_task(t);
}
