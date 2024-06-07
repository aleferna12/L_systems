cmake_minimum_required(VERSION 3.27)
project(L_systems)

set(CMAKE_CXX_STANDARD 17)

add_executable(L_systems src/main.cpp
        src/organism.h
        src/organism.cpp
        src/utility.cpp
        src/utility.h)
