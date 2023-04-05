cmake_minimum_required(VERSION 3.13)

project(sub)

add_library(sub test.c)
target_link_libraries(sub
    pico_stdlib hardware_pio 
)