
global _start
extern kmain

; all information about multiboot :
; https://www.gnu.org/software/grub/manual/multiboot/multiboot.html

%define MULTIBOOT_HEADER_MAGIC 0x1BADB002
%define MULTIBOOT_HEADER_FLAGS 0x00000003
%define MULTIBOOT_HEADER_CHECKSUM -(MULTIBOOT_HEADER_MAGIC+MULTIBOOT_HEADER_FLAGS)

_start:
    jmp start

align 4
multiboot_header:
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_HEADER_CHECKSUM

start:
    push ebx
    call kmain
