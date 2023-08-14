cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_COMPILER "${CMAKE_CURRENT_LIST_DIR}/deps/bin/musl-gcc")

project(tinycat C)

add_executable(tinycat)

add_subdirectory(lib)
add_subdirectory(src)

target_link_libraries(
    tinycat
    PRIVATE
    argparse
)

target_link_options(
    tinycat
    PRIVATE
    -static
    -static-libgcc
)

target_include_directories(
    tinycat
    PRIVATE
    include
)

target_compile_options(
    tinycat
    PRIVATE
    -Wall
    -Wextra
    -std=c17
    $<$<CONFIG:Release,RelWithDebInfo>:-Os>
)

target_compile_definitions(
    tinycat
    PRIVATE
    _POSIX_C_SOURCE=200809L
)
