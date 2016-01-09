#ifndef ASM_H
#define ASM_H

#define cli() asm volatile ("cli");
#define sti() asm volatile ("sti");

#define outb(port,value)              \
    asm volatile ("outb %%al, %%dx"   \
            :                         \
            : "d" (port), "a" (value) \
            :                         \
            );

# define inb(port)                          \
({                                          \
      unsigned char _ret;                   \
      __asm__ (                             \
                "inb %%dx, %%al"            \
                : "=a" (_ret) : "d" (port)  \
              );                            \
      _ret;                                 \
})

#define io_wait()                   \
    asm volatile ("jmp 1f    ;"     \
                  "1: jmp 2f ;"     \
                  "2:");

#define lgdt(table)                      \
    asm volatile ("lgdt %0"              \
                  :                      \
                  : "m" (table)          \
                  : "memory"             \
                 );                      \
    asm volatile ("movw $0x10, %%ax   ;" \
              "movw %%ax, %%ds    ;"     \
              "movw %%ax, %%es    ;"     \
              "movw %%ax, %%fs    ;"     \
              "movw %%ax, %%gs    ;"     \
              "ljmp $0x08, $next  ;"     \
              "next:              ;"     \
             :::);

#define lidt(table)             \
    asm volatile ("lidtl %0"    \
                  :             \
                  : "m" (table) \
                  : "memory"    \
                 );

#endif