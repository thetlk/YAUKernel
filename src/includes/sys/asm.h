#ifndef ASM_H
#define ASM_H

#define sti() asm volatile ("sti");

#define outb(port,value) 						\
    asm volatile ("outb %%al, %%dx"				\
    			  :								\
    			  : "d" (port), "a" (value)		\
    			  :								\
    			  );

#define lgdt(table)						 \
    asm volatile ("lgdt %0"				 \
                  :						 \
                  : "m" (table)			 \
                  : "memory"			 \
                 );						 \
    asm volatile ("movw $0x10, %%ax   ;" \
              "movw %%ax, %%ds    ;"     \
              "movw %%ax, %%es    ;"	 \
              "movw %%ax, %%fs    ;"	 \
              "movw %%ax, %%gs    ;"	 \
              "movw %%ax, %%ss    ;"	 \
              "ljmp $0x08, $next  ;"	 \
              "next:              ;"	 \
             :::);

#define lidt(table) 				\
    asm volatile ("lidtl %0"		\
                  :					\
                  : "m" (table)		\
                  : "memory"		\
                 );

#endif