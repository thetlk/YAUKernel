#ifndef IDT_H
#define IDT_H

#include <driver/pic.h>

struct idt_entry
{
    unsigned short offset_low   : 16; /* offset on segment 15:0 */
    unsigned short segment      : 16; /* segment number         */
    unsigned char always0       : 8;  /* 0000000000000000000000 */
    unsigned char flags         : 8;
    unsigned short offset_high  : 16; /* offset on segment 32:16*/
} __attribute__((packed));

struct idt_register
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

void idt_init(void);
void idt_set(unsigned char n, unsigned short seg, unsigned int offset, unsigned char flags);

#define IDT_NUM 0xFF

#define INT_TYPE_32_TASK_GATE       0x5
#define INT_TYPE_16_INTERRUPT_GATE  0x6
#define INT_TYPE_16_TRAP_GATE       0x7
#define INT_TYPE_32_INTERRUPT_GATE  0xE
#define INT_TYPE_32_TRAP_GATE       0xF

#define INT_PRESENT     (1<<7)
#define INT_DSP_R0      (0<<5)
#define INT_DSP_R1      (1<<5)
#define INT_DSP_R2      (2<<5)
#define INT_DSP_R3      (3<<5)
#define INT_STORAGE_SEG (1<<4)

#define INT_GATE INT_PRESENT | INT_DSP_R0 | INT_TYPE_32_INTERRUPT_GATE
#define TRAP_GATE INT_PRESENT | INT_DSP_R3 | INT_TYPE_32_TRAP_GATE

// exceptions - from intel doc p258
#define INT_DE      0x00    /* divide error exception          */
#define INT_DB      0x01    /* debug exception (trap or fault) */
#define INT_NMI     0x02    /* nonmaskable interrupt           */
#define INT_BP      0x03    /* breakpoint exception            */
#define INT_OF      0x04    /* overflow excetpion              */
#define INT_BR      0x05    /* bound range excedeed exception  */
#define INT_UD      0x06    /* invalid opcode exception        */
#define INT_NM      0x07    /* device not available exception  */
#define INT_DF      0x08    /* double fault exception          */
#define INT_COPROC  0x09    /* coprocessor segment overrun     */
#define INT_TS      0x0A    /* invalid tss exception           */
#define INT_NP      0x0B    /* segment no present              */
#define INT_SS      0x0C    /* stack fault exception           */
#define INT_GP      0x0D    /* general protection fault        */
#define INT_PF      0x0E    /* page fault exception            */
#define INT_MF      0x0F    /* x87 FPU floating point error    */
#define INT_AC      0x10    /* alignment check exception       */
#define INT_MC      0x11    /* machine check exception         */
#define INT_XM      0x12    /* simd floating point exception   */
// IRQs
#define INT_NUM_CLOCK    (MASTER_IDT_OFFSET + 0) /* irq 0 from master */
#define INT_NUM_KEYBOARD (MASTER_IDT_OFFSET + 1) /* irq 1 from master */
// others
#define INT_SYSCALL      0x30

#endif