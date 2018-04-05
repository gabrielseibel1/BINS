cmake_minimum_required(VERSION 3.9)
project(PEPPER C)

set(CMAKE_C_STANDARD 99)

add_executable(
        PEPPER
        src/main.c
        src/lexer.c
        include/lexer.h
        src/table.c
        include/table.h
        src/interpreter.c include/interpreter.h)