#ifndef ASM_H
#define ASM_H

/*
asm ( assembler template
    : output operands                   (optional)
    : input operands                    (optional)
    : clobbered registers list          (optional)
    );

  output operands :
    - a -> eax
    - b -> ebx
    - c -> ecx
    - d -> edx
    - S -> esi
    - D -> edi
    - signe = --> your assembly code does not care about the initial value of the mapped variable

*/

#define cli() asm volatile ("cli");
#define sti() asm volatile ("sti");

#define outb(port, value)             \
    asm volatile ("outb %%dx, %%al"   \
            :                         \
            : "d" (port), "a" (value) \
            :                         \
            );

#define inb(port)                  \
({                                 \
    unsigned char _ret;            \
    asm volatile ("inb %%al, %%dx" \
            : "=a" (_ret)          \
            : "d" (port)           \
            :                      \
        );                         \
    _ret;                          \
})


#define io_wait()               \
    asm volatile ("jmp 1f    ;" \
                  "1: jmp 2f ;" \
                  "2:");

#define lgdt(table, segval)          \
    asm volatile ("lgdt %0"          \
                  :                  \
                  : "m" (table)      \
                  : "memory"         \
                 );                  \
    asm volatile (                   \
              "movw %%ds, %%ax    ;" \
              "movw %%es, %%ax    ;" \
              "movw %%fs, %%ax    ;" \
              "movw %%gs, %%ax    ;" \
              "ljmp 0x08:next     ;" \
              "next:              ;" \
             :                       \
             : "a" (segval)          \
             :                       \
             );

#define lidt(table)             \
    asm volatile ("lidt %0"     \
                  :             \
                  : "m" (table) \
                  :             \
                 );

#define ltr(val)             \
    asm volatile ("ltr %%ax" \
            :                \
            : "a" (val)      \
            :                \
        );

#define set_ss_esp()                   \
    asm volatile("mov %%eax, 0x18   ;" \
                "movw %%ss, %%ax    ;" \
                "mov %%esp, 0x20000 ;" \
         ::: );

#define mov_cr3(val)                \
    asm volatile("mov %%cr3, %%eax" \
      :                             \
      : "a" (val)                   \
      :                             \
    );

#define cr0_enable_paging()            \
    asm volatile("mov %%eax, %%cr0  ;" \
        "or %%eax, 0x80000000       ;" \
        "mov %%cr0, %%eax           ;" \
        :::                            \
    );

#define cr4_set_pse_flag()              \
    asm volatile("mov %%eax, %%cr4  ;"  \
              "or %%eax, 0x00000010 ;"  \
              "mov %%cr4, %%eax     ;"  \
              :::                       \
            );

#endif