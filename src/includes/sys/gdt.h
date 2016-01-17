#ifndef I386_GDT_H
#define I386_GDT_H

struct gdt_entry /* page 107 intel doc */
{
    unsigned short limit_low  : 16; /* segment limit 15:0                   */
    unsigned short base_low   : 16; /* base address 15:0                    */
    unsigned char base_high_1 : 8;  /* base address 23:16                   */
    /* access */
    unsigned char type        : 4;  /* segment type                         */
    unsigned char s           : 1;  /* descriptor type                      */
    unsigned char dpl         : 2;  /* descriptor privilege level           */
    unsigned char p           : 1;  /* segment present                      */
    /* end access */
    unsigned char limit_high  : 4;  /* segment limit 19:16                  */
    /* others */
    unsigned char avl         : 1;  /* available for use by system software */
    unsigned char l           : 1;  /* 64bit code segment                   */
    unsigned char db          : 1;  /* default operation size               */
    unsigned char g           : 1;  /* granularity                          */
    /* end others */
    unsigned char base_high_2 : 8;  /* base address 31:24                   */
} __attribute__((packed));

struct gdt_register
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

void gdt_init();

#define LIMIT_LOW(limit) (limit & 0xFFFF)
#define LIMIT_HIGH(limit) ((limit >> 16) & 0xFF)
#define BASE_LOW(base) (base & 0xFFFF)
#define BASE_HIGH_1(base) ((base >> 16) & 0xFF)
#define BASE_HIGH_2(base) ((base >> 24) & 0xFF)

#define TYPE_CODE_SEG               (1<<3)
#define TYPE_CODE_SEG_CONFORMING    (1<<2)
#define TYPE_CODE_SEG_READABLE      (1<<1)
#define TYPE_CODE_SEG_ACCESSED      (1<<0)

#define TYPE_DATA_SEG               (0<<3)
#define TYPE_DATA_SEG_GROWS_DOWN    (1<<2)
#define TYPE_DATA_SEG_READABLE      (1<<1)
#define TYPE_DATA_SEG_ACCESSED      (1<<0)

#define DESCRIPTOR_SYSTEM    0
#define DESCRIPTOR_CODE_DATA 1

#define DPL_RING_0 0
#define DPL_RING_1 1
#define DPL_RING_2 2
#define DPL_RING_3 3

#define IS_NOT_PRESENT_IN_MEMORY 0
#define IS_PRESENT_IN_MEMORY     1

#define NOT_AVAILABLE 0
#define AVAILABLE 1

#define LONG_MODE_OFF 0
#define LONG_MODE_ON 1

#define OP_SIZE_16 0
#define OP_SIZE_32 1

#define LIMIT_IN_BYTE 0
#define LIMIT_IN_4K   1


#endif