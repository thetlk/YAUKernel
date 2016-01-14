#include <sys/gdt.h>
#include <sys/asm.h>
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
        .type        = TYPE_DATA_SEG | TYPE_DATA_SEG_READABLE | TYPE_DATA_SEG_ACCESSED | TYPE_DATA_SEG_GROWS_DOWN,
        .s           = DESCRIPTOR_CODE_DATA,
        .dpl         = DPL_RING_0,
        .p           = IS_PRESENT_IN_MEMORY,
        .limit_high  = LIMIT_HIGH(0),
        .avl         = AVAILABLE,
        .l           = LONG_MODE_OFF,
        .db          = OP_SIZE_32,
        .g           = LIMIT_IN_4K,
        .base_high_2 = BASE_HIGH_2(0)
    },
    /* segment 0x20 - user code segment: base = 0x30000, limit = 1 (4096) */
    {
        .limit_low   = LIMIT_LOW(0xFFFFF),
        .base_low    = BASE_LOW(0x0),
        .base_high_1 = BASE_HIGH_1(0x0),
        .type        = TYPE_CODE_SEG | TYPE_CODE_SEG_READABLE | TYPE_CODE_SEG_ACCESSED | TYPE_CODE_SEG_CONFORMING,
        .s           = DESCRIPTOR_CODE_DATA,
        .dpl         = DPL_RING_3,
        .p           = IS_PRESENT_IN_MEMORY,
        .limit_high  = LIMIT_HIGH(0xFFFFF),
        .avl         = AVAILABLE,
        .l           = LONG_MODE_OFF,
        .db          = OP_SIZE_32,
        .g           = LIMIT_IN_4K,
        .base_high_2 = BASE_HIGH_2(0x0)
    },
    /* segment 0x28 - user data segment: base = 0x3000, limit = 1 (4096) */
    {
        .limit_low   = LIMIT_LOW(0xFFFFF),
        .base_low    = BASE_LOW(0x0),
        .base_high_1 = BASE_HIGH_1(0x0),
        .type        = TYPE_DATA_SEG | TYPE_DATA_SEG_READABLE | TYPE_DATA_SEG_ACCESSED,
        .s           = DESCRIPTOR_CODE_DATA,
        .dpl         = DPL_RING_3,
        .p           = IS_PRESENT_IN_MEMORY,
        .limit_high  = LIMIT_HIGH(0xFFFFF),
        .avl         = AVAILABLE,
        .l           = LONG_MODE_OFF,
        .db          = OP_SIZE_32,
        .g           = LIMIT_IN_4K,
        .base_high_2 = BASE_HIGH_2(0x0)
    },
    /* segment 0x30 - user stack segment: base = 0x0, limit = 0x0 */
    {
        .limit_low   = LIMIT_LOW(0x0),
        .base_low    = BASE_LOW(0x0),
        .base_high_1 = BASE_HIGH_1(0x0),
        .type        = TYPE_DATA_SEG | TYPE_DATA_SEG_READABLE | TYPE_DATA_SEG_ACCESSED | TYPE_DATA_SEG_GROWS_DOWN,
        .s           = DESCRIPTOR_CODE_DATA,
        .dpl         = DPL_RING_3,
        .p           = IS_PRESENT_IN_MEMORY,
        .limit_high  = LIMIT_HIGH(0x0),
        .avl         = AVAILABLE,
        .l           = LONG_MODE_OFF,
        .db          = OP_SIZE_32,
        .g           = LIMIT_IN_4K,
        .base_high_2 = BASE_HIGH_2(0x0)
    },
    /* TSS segment*/
    {
        .limit_low   = LIMIT_LOW(sizeof(struct tss)),
        .base_low    = BASE_LOW(0),
        .base_high_1 = BASE_HIGH_1(0),
        .type        = TYPE_CODE_SEG | TYPE_CODE_SEG_ACCESSED,
        .s           = DESCRIPTOR_SYSTEM,
        .dpl         = DPL_RING_3,
        .p           = IS_PRESENT_IN_MEMORY,
        .limit_high  = LIMIT_HIGH(sizeof(struct tss)),
        .avl         = NOT_AVAILABLE,
        .l           = LONG_MODE_OFF,
        .db          = OP_SIZE_16,
        .g           = LIMIT_IN_BYTE,
        .base_high_2 = BASE_HIGH_2(0)
    }
};

static struct gdt_register kernel_gdt_register = {0, 0};
static struct tss kernel_tss = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void gdt_init()
{

    // init kernel tss
    kernel_tss.ss0 = 0x18; // stack kernel entry in gdt
    kernel_tss.esp0 = 0x20000;

    // init kernel tss entry in gdt (base only)
    kernel_gdt[7].base_low = BASE_LOW((unsigned int) &kernel_tss);
    kernel_gdt[7].base_high_1 = BASE_HIGH_1((unsigned int) &kernel_tss);
    kernel_gdt[7].base_high_2 = BASE_HIGH_2((unsigned int) &kernel_tss);

    // init gdt register
    kernel_gdt_register.limit = sizeof(kernel_gdt);
    kernel_gdt_register.base = (unsigned int) &kernel_gdt[0];

    // load gdt register
    video_print("Init GDT ... ");
    lgdt(kernel_gdt_register, 0x10); // data at entry 0x10
    video_print_ok();

}
