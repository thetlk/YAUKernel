#include <sys/scheduler.h>
#include <sys/interrupts.h>
#include <sys/task.h>
#include <sys/tss.h>
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

void scheduler_switch_to_task(struct task *t)
{
    unsigned int kesp;
    unsigned short kss;

    // update tss
    tss_update(t->kstack.ss0, t->kstack.esp0);

    t->regs.eflags |= 0x200; // set IF flag
    t->regs.eflags &= 0xFFFFBFFF; // removes NT flag

    if(t->regs.cs == 0x08)
    {
        // inside kernel restore last stack
        kesp = t->regs.esp;
        kss = t->regs.ss;
    } else {
        // inside user task - take clean kernel stack
        kesp = t->kstack.esp0;
        kss = t->kstack.ss0;
    }

    asm volatile(
        "mov %%ss, %[kss]   ;"
        "mov %%esp, %[kesp] ;"
        "cmp %[mode], 0x08  ;"
        "je kernel_mode     ;"
        "push %[ss]         ;"  // user mode needs cs and esp
        "push %[esp]        ;"  // while kernel mode doesn't
        "kernel_mode:       ;"
        "push %[eflags]     ;"
        "push %[cs]         ;"
        "push %[eip]        ;"
        "push %[eax]        ;"
        "push %[ebx]        ;"
        "push %[ecx]        ;"
        "push %[edx]        ;"
        "push %[ebp]        ;"
        "push %[esi]        ;"
        "push %[edi]        ;"
        "push %[ds]         ;"
        "push %[es]         ;"
        "push %[fs]         ;"
        "push %[gs]         ;"
        "mov al, 0x20       ;"
        "out 0x20, al       ;"
        "mov %%eax, %[cr3]  ;"
        "mov %%cr3, %%eax   ;"
        "pop %%gs           ;"
        "pop %%fs           ;"
        "pop %%es           ;"
        "pop %%ds           ;"
        "pop %%edi          ;"
        "pop %%esi          ;"
        "pop %%ebp          ;"
        "pop %%edx          ;"
        "pop %%ecx          ;"
        "pop %%ebx          ;"
        "pop %%eax          ;"
        "iret"
        :
        : [kss]     "m" (kss),
          [kesp]    "m" (kesp),
          [mode]    "m" (t->regs.cs),
          [ss]      "m" (t->regs.ss),
          [esp]     "m" (t->regs.esp),
          [eflags]  "m" (t->regs.eflags),
          [cs]      "m" (t->regs.cs),
          [eip]     "m" (t->regs.eip),
          [eax]     "m" (t->regs.eax),
          [ebx]     "m" (t->regs.ebx),
          [ecx]     "m" (t->regs.ecx),
          [edx]     "m" (t->regs.edx),
          [ebp]     "m" (t->regs.ebp),
          [esi]     "m" (t->regs.esi),
          [edi]     "m" (t->regs.edi),
          [ds]      "m" (t->regs.ds),
          [es]      "m" (t->regs.es),
          [fs]      "m" (t->regs.fs),
          [gs]      "m" (t->regs.gs),
          [cr3]     "m" (t->regs.cr3)
        : "eax"
    );
}

/*
If interrupted inside kernel code :
    eflags
    cs
    eip
    eax
    ecx
    edx
    ebx
    esp_kernel
    ebp
    esi
    edi
    ds
    es
    fs
    gs          <--- regs (pushed_registers)
If interrupted inside user code :
    ss
    esp
    eflags
    cs
    eip
    eax
    ecx
    edx
    ebx
    esp_kernel
    ebp
    esi
    edi
    ds
    es
    fs
    gs          <--- regs (pushed_registers)
*/

void schedule(struct pushed_registers *regs)
{
    struct tss *ktss = tss_get();

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

        // task interrupted inside kernel code
        if(current_task->regs.cs == 0x08)
        {
            current_task->regs.ss = ktss->ss0;
            current_task->regs.esp = regs->esp_kernel + 3*4; // ebx, edx, ecx, eax
        } else { // interrupted inside user code
            current_task->regs.ss = regs->ss;
            current_task->regs.esp = regs->esp;
        }

        // save tss state
        current_task->kstack.ss0 = ktss->ss0;
        current_task->kstack.esp0 = ktss->esp0;

        // simple roundrobin - just takes next task
        current_task = &task_list[(current_task->pid + 1)%n_task];
    } else {
        return; // do nothing - current_task != 0 && ntask == 1
    }

    // launch current_task here
    scheduler_switch_to_task(current_task);
}
