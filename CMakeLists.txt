cmake_minimum_required(VERSION 3.27)
project(L_systems)

set(CMAKE_CXX_STANDARD 23)

add_executable(
    L_systems
    src/main.cpp
    src/tree.h
    src/tree.cpp
    src/utility.cpp
    src/utility.h
    src/forest.cpp
    src/forest.h
        src/params.h
        src/genome.cpp
        src/genome.h
)
