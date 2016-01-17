cmake_minimum_required(VERSION 3.0)

set(boot_source boot.asm multiboot.c)

add_library(boot ${boot_source})
