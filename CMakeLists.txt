cmake_minimum_required(VERSION 3.27)
project(L_systems)

set(CMAKE_CXX_STANDARD 23)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(
    L_systems
    src/main.cpp
    src/tree.h
    src/tree.cpp
    src/utility.cpp
    src/utility.h
    src/forest.cpp
    src/forest.h
        src/parameters.h
        src/genome.cpp
        src/genome.h
        src/model.cpp
        src/model.h
)
