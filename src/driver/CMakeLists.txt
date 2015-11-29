cmake_minimum_required(VERSION 3.4)

file(GLOB driver_source *.c)

add_library(driver ${driver_source})
