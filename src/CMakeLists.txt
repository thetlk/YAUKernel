cmake_minimum_required(VERSION 3.4)

file(GLOB kernel_source *.c)

add_executable(kernel boot.asm ${kernel_source})

