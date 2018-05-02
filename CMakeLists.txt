cmake_minimum_required(VERSION 3.9)
project(BINS)

set(CMAKE_C_STANDARD 99)

add_executable(
        BINS
        src/main.c
        src/lexer.c
        include/lexer.h
        src/table.c
        include/table.h
        src/interpreter.c
        include/interpreter.h
        src/node_map.cpp
        include/node_map.h
        src/stamp.cpp src/stamp.h)