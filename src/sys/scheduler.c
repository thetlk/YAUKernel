#include <sys/scheduler.h>
#include <sys/task.h>
#include <driver/video.h>

struct task task_list[32];
struct task *current_task = 0;
unsigned int n_task = 0;

void scheduler_add_task(struct task t)
{
    task_list[n_task] = t;
    task_list[n_task].pid = n_task;
    n_task++;
}

void schedule(struct pushed_registers *regs)
{

    if(n_task == 0)
    {
        return;
    }

    if(current_task == 0 && n_task >= 1)
    {
        current_task = &task_list[0];
    }
    else if(n_task > 1)
    {
        // save context of current task
        current_task->regs.gs = regs->gs;
        current_task->regs.fs = regs->fs;
        current_task->regs.es = regs->es;
        current_task->regs.ds = regs->ds;
        current_task->regs.edi = regs->edi;
        current_task->regs.esi = regs->esi;
        current_task->regs.ebp = regs->ebp;
        current_task->regs.esp = regs->esp;
        current_task->regs.ebx = regs->ebx;
        current_task->regs.edx = regs->edx;
        current_task->regs.ecx = regs->ecx;
        current_task->regs.eax = regs->eax;
        current_task->regs.eip = regs->eip;
        current_task->regs.cs = regs->cs;
        current_task->regs.eflags = regs->eflags;
        current_task->regs.esp = regs->esp;
        current_task->regs.ss = regs->ss;

        // simple roundrobin - just takes next task
        current_task = &task_list[(current_task->pid + 1)%n_task];
    } else {
        return; // do nothing - current_task != 0 && ntask == 1
    }

    // launch current_task here
    current_task->regs.eflags |= 0x200; // set IF flag
    current_task->regs.eflags &= 0xFFFFBFFF; // removes NT flag
    asm volatile(
        "push %[ss] ;"
        "push %[esp] ;"
        "push %[eflags] ;"
        "push %[cs] ;"
        "push %[eip] ;"
        "push %[eax]  ;"
        "push %[ebx]  ;"
        "push %[ecx]  ;"
        "push %[edx]  ;"
        "push %[ebp]  ;"
        "push %[esi]  ;"
        "push %[edi]  ;"
        "push %[ds]   ;"
        "push %[es]   ;"
        "push %[fs]   ;"
        "push %[gs]   ;"
        "mov al, 0x20 ;"
        "out 0x20, al ;"
        "mov %%eax, %[cr3] ;"
        "mov %%cr3, %%eax ;"
        "pop %%gs  ;"
        "pop %%fs  ;"
        "pop %%es  ;"
        "pop %%ds  ;"
        "pop %%edi  ;"
        "pop %%esi  ;"
        "pop %%ebp  ;"
        "pop %%edx ;"
        "pop %%ecx   ;"
        "pop %%ebx   ;"
        "pop %%eax   ;"
        "iret"
        :
        : [ss] "m" (current_task->regs.ss),
          [esp] "m" (current_task->regs.esp),
          [eflags] "m" (current_task->regs.eflags),
          [cs] "m" (current_task->regs.cs),
          [eip] "m" (current_task->regs.eip),
          [eax] "m" (current_task->regs.eax),
          [ebx] "m" (current_task->regs.ebx),
          [ecx] "m" (current_task->regs.ecx),
          [edx] "m" (current_task->regs.edx),
          [ebp] "m" (current_task->regs.ebp),
          [esi] "m" (current_task->regs.esi),
          [edi] "m" (current_task->regs.edi),
          [ds] "m" (current_task->regs.ds),
          [es] "m" (current_task->regs.es),
          [fs] "m" (current_task->regs.fs),
          [gs] "m" (current_task->regs.gs),
          [cr3] "m" (current_task->regs.cr3)
        : "eax", "esp"
    );

}
