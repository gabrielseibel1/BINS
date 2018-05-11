cmake_minimum_required(VERSION 3.9)
project(BINS)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        BINS
        src/main.cpp
        src/SpiceReader.cpp
        include/SpiceReader.h
        src/table.cpp
        include/table.h
        src/SpiceInterpreter.cpp
        include/SpiceInterpreter.h
        src/NodeMap.cpp
        include/NodeMap.h
        src/MatrixManager.cpp
        include/MatrixManager.h)