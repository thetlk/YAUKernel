#include <boot/multiboot.h>
#include <driver/video.h>

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

void multiboot_display(struct multiboot_info *mbi)
{
    video_printf("multiboot infos :\n");
    video_printf("\t- flags = 0x%x\n", mbi->flags);

    if(CHECK_FLAG(mbi->flags, FLAG_MEM))
    {
        video_printf("\t- mem_lower = %uKB, mem_upper = %uKB\n", mbi->mem_lower, mbi->mem_upper);
    }

    if(CHECK_FLAG(mbi->flags, FLAG_BOOT_DEVICE))
    {
        video_printf("\t- boot_device = 0x%x\n", mbi->boot_device);
    }

    if(CHECK_FLAG(mbi->flags, FLAG_CMDLINE))
    {
        video_printf("\t- cmdline = %s\n", (char *) mbi->cmdline);
    }

    if(CHECK_FLAG(mbi->flags, FLAG_MODS))
    {
        struct multiboot_module *mod = (struct multiboot_module *) mbi->mods_addr;
        int i;
        video_printf("\t- mods_count = %d, mods_addr = %p\n", mbi->mods_count, mbi->mods_addr);

        for(i=0; i<mbi->mods_count; i++)
        {
            video_printf("\t\t- mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
                mod->mod_start,
                mod->mod_end,
                mod->cmdline);
            mod++;
        }
    }

    if(CHECK_FLAG(mbi->flags, FLAG_MMAP))
    {
        struct multiboot_memory_map *mmap = (struct multiboot_memory_map *) mbi->mmap_addr;

        video_printf("\t- memory map :\n");
        while((unsigned int) mmap < mbi->mmap_addr + mbi->mmap_length)
        {
            video_printf("\t\t- 0x%lx -- 0x%lx (0x%lx) %s (0x%x)\n",
                mmap->base,
                mmap->base + mmap->length,
                mmap->length,
                (mmap->type == 1) ? "RAM" : "reserved",
                mmap->type);

            mmap = (struct multiboot_memory_map *) ((unsigned int) mmap + mmap->size + sizeof(mmap->size));
        }
    }

}
