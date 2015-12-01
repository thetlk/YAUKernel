cmake_minimum_required(VERSION 3.4)

file(GLOB i386_source *.c)

add_library(i386 ${i386_source})
