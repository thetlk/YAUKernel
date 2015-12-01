#include <sys/gdt.h>
#include <driver/video.h>

static struct gdt_entry kernel_gdt[] = {
    /* segment 0x00 - null entry */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    /* segment 0x08 - code segment : base = 0, limit = 0xFFFFF */
    {
        .limit_low   = LIMIT_LOW(0xFFFFF),
        .base_low    = BASE_LOW(0),
        .base_high_1 = BASE_HIGH_1(0),
        .type        = TYPE_CODE_SEG | TYPE_CODE_SEG_READABLE | TYPE_CODE_SEG_ACCESSED,
        .s           = DESCRIPTOR_CODE_DATA,
        .dpl         = DPL_RING_0,
        .p           = IS_PRESENT_IN_MEMORY,
        .limit_high  = LIMIT_HIGH(0xFFFFF),
        .avl         = AVAILABLE,
        .l           = LONG_MODE_OFF,
        .db          = OP_SIZE_32,
        .g           = LIMIT_IN_4K,
        .base_high_2 = BASE_HIGH_2(0)
    },
    /* segment 0x10 - data segment: base = 0, limit = 0xFFFFF */
    {
        .limit_low   = LIMIT_LOW(0xFFFFF),
        .base_low    = BASE_LOW(0),
        .base_high_1 = BASE_HIGH_1(0),
        .type        = TYPE_DATA_SEG | TYPE_DATA_SEG_READABLE | TYPE_DATA_SEG_ACCESSED,
        .s           = DESCRIPTOR_CODE_DATA,
        .dpl         = DPL_RING_0,
        .p           = IS_PRESENT_IN_MEMORY,
        .limit_high  = LIMIT_HIGH(0xFFFFF),
        .avl         = AVAILABLE,
        .l           = LONG_MODE_OFF,
        .db          = OP_SIZE_32,
        .g           = LIMIT_IN_4K,
        .base_high_2 = BASE_HIGH_2(0)
    },
    /* segment 0x18 - stack segment: base = 0, limit = 0 */
    {
        .limit_low   = LIMIT_LOW(0),
        .base_low    = BASE_LOW(0),
        .base_high_1 = BASE_HIGH_1(0),
        .type        = TYPE_DATA_SEG | TYPE_DATA_SEG_CONFORMING | TYPE_DATA_SEG_READABLE | TYPE_DATA_SEG_ACCESSED,
        .s           = DESCRIPTOR_CODE_DATA,
        .dpl         = DPL_RING_0,
        .p           = IS_PRESENT_IN_MEMORY,
        .limit_high  = LIMIT_HIGH(0),
        .avl         = AVAILABLE,
        .l           = LONG_MODE_OFF,
        .db          = OP_SIZE_32,
        .g           = LIMIT_IN_4K,
        .base_high_2 = BASE_HIGH_2(0)
    }
};

static struct gdt_register kernel_gdt_register = {0, 0};

void gdt_init()
{
    // init gdt register
    kernel_gdt_register.limit = sizeof(kernel_gdt);
    kernel_gdt_register.base = (unsigned int) &kernel_gdt[0];

    // load gdt register
    video_print("Init GDT ... ");
    asm volatile ("lgdt %0"
                  :
                  : "m" (kernel_gdt_register)
                  : "memory"
                 );
    asm volatile ("movw $0x10, %%ax   ;"
                  "movw %%ax, %%ds    ;"
                  "movw %%ax, %%es    ;"
                  "movw %%ax, %%fs    ;"
                  "movw %%ax, %%gs    ;"
                  "ljmp $0x08, $next  ;"
                  "next:              ;"
                 :::);
    video_print_ok();

}
