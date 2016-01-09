#include <sys/idt.h>
#include <sys/interrupts.h>
#include <sys/asm.h>
#include <driver/video.h>

struct idt_entry idt_list[IDT_NUM];
struct idt_register idt_register;

void idt_set(unsigned char n, unsigned short seg, unsigned int offset, unsigned char flags)
{
    idt_list[n].segment = seg;
    idt_list[n].offset_low = offset & 0xFFFF;
    idt_list[n].offset_high = (offset >> 16) & 0xFFFF;
    idt_list[n].always0 = 0;
    idt_list[n].flags = flags;
}

void idt_init()
{
    int i;

    for(i=0; i<IDT_NUM; i++)
    {
        idt_set(i, 0x08, (unsigned int) _asm_default_interrupt, INT_GATE);
    }

    // idt_set(INT_NUM_CLOCK, 0x08, (unsigned int) _asm_irq_0, INT_GATE);
    idt_set(INT_NUM_KEYBOARD, 0x08, (unsigned int) _asm_irq_1, INT_GATE);

    idt_register.limit = sizeof(struct idt_entry) * IDT_NUM;
    idt_register.base = (unsigned int) &idt_list[0];

    video_print("Init IDT ... ");
    lidt(idt_register);
    video_print_ok();

}
