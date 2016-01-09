cmake_minimum_required(VERSION 3.4)

file(GLOB libc_source *.c)

add_library(libc ${libc_source})
