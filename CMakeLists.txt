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

include(pico_sdk_import.cmake)
include(pimoroni_pico_import.cmake)

project(L_systems C CXX ASM)

pico_sdk_init()

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
    src/parameters.h
    src/genome.cpp
    src/genome.h
    src/model.cpp
    src/model.h
    src/pos.h
    src/generaltree.h
    src/graphics.h
    src/graphics.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(L_systems PRIVATE -O3)
elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(L_systems PRIVATE -g)
elseif (CMAKE_BUILD_TYPE STREQUAL "Profile")
    target_compile_options(L_systems PRIVATE -g -pg -no-pie)
elseif (CMAKE_BUILD_TYPE STREQUAL "ProfileO3")
    target_compile_options(L_systems PRIVATE -g -O3 -pg -no-pie)
endif()

target_compile_options(L_systems PRIVATE -Wall -Wextra)
target_compile_definitions(L_systems PRIVATE PICO_RAND_SEED_ENTROPY_SRC_ROSC=1)
get_target_property(COMPILE_DEFS L_systems COMPILE_DEFINITIONS)
get_target_property(COMPILE_OPS L_systems COMPILE_OPTIONS)
message(STATUS "Compilation definitions: ${COMPILE_DEFS}")
message(STATUS "Compilation definitions: ${COMPILE_OPS}")

include(common/pimoroni_bus)
include(libraries/pico_display_2/pico_display_2)
include(libraries/pico_graphics/pico_graphics)
include(drivers/st7789/st7789)

target_link_libraries(L_systems
        pico_stdlib
        pico_rand
        pico_cyw43_arch_none
        pico_display_2
        pico_graphics
        st7789
)

pico_enable_stdio_usb(L_systems 1)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(L_systems)
