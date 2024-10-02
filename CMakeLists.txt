cmake_minimum_required(VERSION 3.27)
project(L_systems)

set(CMAKE_CXX_STANDARD 23)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# TODO: use add_compile_options and target_compile_options instead
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
elseif (CMAKE_BUILD_TYPE STREQUAL "Profile")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pg -no-pie")
elseif (CMAKE_BUILD_TYPE STREQUAL "ProfileO3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -pg -no-pie")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

message("Compilation flags: " ${CMAKE_CXX_FLAGS})

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
    src/pos.h
)
