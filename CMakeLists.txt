cmake_minimum_required(VERSION 3.27)

# Below has to be done before project directive
set(PICO_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/pico-sdk")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (NOT PICO_BOARD)
    message("Assuming PICO_BOARD=pico_w")
    set(PICO_BOARD "pico_w")
endif()

include("${PICO_SDK_PATH}/external/pico_sdk_import.cmake")

project(L_systems)

pico_sdk_init()

set(CMAKE_CXX_STANDARD 23)

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
    src/generaltree.h
)

target_compile_definitions(L_systems PRIVATE PICO_RAND_SEED_ENTROPY_SRC_ROSC=1)

target_link_libraries(L_systems
        pico_stdlib
        pico_rand
        pico_cyw43_arch_none
)

pico_enable_stdio_usb(L_systems 1)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(L_systems)
