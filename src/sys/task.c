#include <sys/task.h>
#include <sys/pagemem.h>
#include <sys/scheduler.h>
#include <sys/memory.h>
#include <sys/asm.h>
#include <sys/kmalloc.h>
#include <libc/string.h>
#include <driver/video.h>

struct task *task_load(void *function, unsigned int size)
{
    struct page_directory *page_directory;
    struct page_list *page_list;
    struct page_list *tmp;
    struct page *kernel_stack;
    struct task *t;

    page_directory = pagemem_pagedirectory_create();
    mov_cr3(page_directory->base->physaddr);

    // map USER_SPACE_BASE_ADDR && copy code
    page_list = pagemem_pagedirectory_map(page_directory, (void *) USER_SPACE_BASE_ADDR, size);
    memcpy((void*) USER_SPACE_BASE_ADDR, function, size);

    // create user stack
    tmp = pagemem_pagedirectory_map(page_directory, (void *) USER_SPACE_STACK_ADDR, USER_SPACE_STACK_SIZE);
    LIST_CONCAT(page_list, tmp, next_page);
    kfree(tmp);

    // create kernel stack
    kernel_stack = memory_get_page_heap();

    t = kmalloc(sizeof(struct task));
    t->kstack.ss0 = SEGMENT_KERNEL_STACK;
    t->kstack.esp0 = (unsigned int) kernel_stack->virtaddr + PAGE_SIZE - 16;
    t->regs.ss = SEGMENT_USER_STACK | 3;
    t->regs.esp = USER_SPACE_STACK_ADDR + USER_SPACE_STACK_SIZE - 16;
    t->regs.eflags = 0x0;
    t->regs.cs = SEGMENT_USER_CODE | 3;
    t->regs.eip = USER_SPACE_BASE_ADDR;
    t->regs.ds = SEGMENT_USER_DATA | 3;
    t->regs.fs = SEGMENT_USER_DATA | 3;
    t->regs.es = SEGMENT_USER_DATA | 3;
    t->regs.gs = SEGMENT_USER_DATA | 3;
    t->regs.cr3 = (unsigned int) page_directory->base->physaddr;
    t->regs.eax = 0;
    t->regs.ebx = 0;
    t->regs.ecx = 0;
    t->regs.edx = 0;
    t->regs.esi = 0;
    t->regs.edi = 0;
    t->regs.ebp = 0;

    t->page_directory = page_directory;
    t->page_list = page_list;

    return t;
}

void task_load_and_schedule(void *function, unsigned int size)
{
    struct task *t;

    t = task_load(function, size);
    scheduler_add_task(t);
}
