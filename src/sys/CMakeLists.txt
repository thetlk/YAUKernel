cmake_minimum_required(VERSION 3.0)

file(GLOB sys_source *.c)

add_library(sys ${sys_source} interrupts.asm)
