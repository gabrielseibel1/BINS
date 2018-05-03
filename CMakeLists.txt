cmake_minimum_required(VERSION 3.9)
project(BINS)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        BINS
        src/main.cpp
        src/lexer.cpp
        include/lexer.h
        src/table.cpp
        include/table.h
        src/interpreter.cpp
        include/interpreter.h
        src/node_map.cpp
        include/node_map.h
        src/MatrixManager.cpp
        include/MatrixManager.h)