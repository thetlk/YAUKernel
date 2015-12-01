cmake_minimum_required(VERSION 3.4)

file(GLOB kernel_source *.c)

include_directories(includes)
add_subdirectory(driver)
add_subdirectory(i386)

add_executable(kernel boot.asm ${kernel_source})
target_link_libraries(kernel i386 driver)
