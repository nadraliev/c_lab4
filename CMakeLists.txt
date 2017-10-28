cmake_minimum_required(VERSION 3.8)
project(lab4)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

set(SOURCE_FILES main.c)
add_executable(lab4 ${SOURCE_FILES})