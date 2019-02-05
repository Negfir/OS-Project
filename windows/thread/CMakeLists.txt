cmake_minimum_required(VERSION 3.8)
project(thread)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(thread ${SOURCE_FILES})