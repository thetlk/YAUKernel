cmake_minimum_required(VERSION 3.0)


include_directories(includes)
add_subdirectory(driver)
add_subdirectory(boot)
add_subdirectory(sys)
add_subdirectory(libc)

# add_executable(kernel boot.asm ${kernel_source})
add_executable(kernel kmain.c)
target_link_libraries(kernel boot sys driver libc)
