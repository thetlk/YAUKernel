
global _start
extern kmain

; all information about multiboot :
; https://www.gnu.org/software/grub/manual/multiboot/multiboot.html

%define MULTIBOOT_ALIGN 0x1
%define MULTIBOOT_MEMINFO 0x2

%define MULTIBOOT_HEADER_MAGIC 0x1BADB002
%define MULTIBOOT_HEADER_FLAGS (MULTIBOOT_ALIGN | MULTIBOOT_MEMINFO)
%define MULTIBOOT_HEADER_CHECKSUM -(MULTIBOOT_HEADER_MAGIC+MULTIBOOT_HEADER_FLAGS)

section .multiboot
    align 4
    multiboot_header:
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_HEADER_CHECKSUM

section .bootstrap_stack
    stack_bottom:
        times 16384 db 0
    stack_top:

_start:
    cli
    mov esp, stack_top
    push ebx
    push eax
    call kmain
