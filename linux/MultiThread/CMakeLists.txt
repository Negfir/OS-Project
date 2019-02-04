cmake_minimum_required(VERSION 3.8)
project(MultiThread)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(MultiThread ${SOURCE_FILES})
SET( CMAKE_CXX_FLAGS  "<other compiler flags> -pthread")
target_link_libraries(${PROJECT_NAME} pthread)
