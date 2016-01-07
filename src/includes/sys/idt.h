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

void init_idt();

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

#define INT_NUM_CLOCK    (MASTER_IDT_OFFSET + 0) /* irq 0 from master */
#define INT_NUM_KEYBOARD (MASTER_IDT_OFFSET + 1) /* irq 1 from master */

#endif