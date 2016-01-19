#ifndef MBI_H
#define MBI_H

/*
https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
*/

#define MULTIBOOT_HEADER_MAGIC      0x1BADB002
#define MULTIBOOT_BOOTLOADER_MAGIC  0x2BADB002

#define FLAG_MEM 0
#define FLAG_BOOT_DEVICE 1
#define FLAG_CMDLINE 2
#define FLAG_MODS 3
#define FLAG_AOUT_SYMBOL 4
#define FLAG_ELF_HEADER 5
#define FLAG_MMAP 6

struct multiboot_aout_symbol_table
{
    unsigned int tabsize;
    unsigned int strsize;
    unsigned int addr;
    unsigned int reserved;
};

struct multiboot_elf_section_header_table
{
    unsigned int num;
    unsigned int size;
    unsigned int addr;
    unsigned int shndx;
};

struct multiboot_module
{
    /* the memory used goes from bytes 'mod_start' to 'mod_end-1' inclusive */
    unsigned int mod_start;
    unsigned int mod_end;

    /* Module command line */
    unsigned int cmdline;

    /* padding to take it to 16 bytes (must be zero) */
    unsigned int pad;
};

struct multiboot_memory_map
{
    unsigned int size;
    unsigned int base_addr_low, base_addr_high;
    unsigned int length_low, length_high;
    unsigned int type;
};

struct multiboot_info
{
    /* Multiboot info version number */
    unsigned int flags;

    /* Available memory from BIOS */
    unsigned int mem_lower;
    unsigned int mem_upper;

    /* "root" partition */
    unsigned int boot_device;

    /* Kernel command line */
    unsigned int cmdline;

    /* Boot-Module list */
    unsigned int mods_count;
    unsigned int mods_addr;

    union
    {
        struct multiboot_aout_symbol_table aout_sym;
        struct multiboot_elf_section_header_table elf_sec;
    } u;

    /* Memory Mapping buffer */
    unsigned int mmap_length;
    unsigned int mmap_addr;

    /* Drive Info buffer */
    unsigned int drives_length;
    unsigned int drives_addr;

    /* ROM configuration table */
    unsigned int config_table;

    /* Boot Loader Name */
    unsigned int boot_loader_name;

    /* APM table */
    unsigned int apm_table;

    /* Video */
    unsigned int vbe_control_info;
    unsigned int vbe_mode_info;
    unsigned short vbe_mode;
    unsigned short vbe_interface_seg;
    unsigned short vbe_interface_off;
    unsigned short vbe_interface_len;
};

void multiboot_display(struct multiboot_info *mbi);

#endif