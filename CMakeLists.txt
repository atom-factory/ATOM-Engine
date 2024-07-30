cmake_minimum_required(VERSION 3.14)
project(ATOM)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(VCPKG_TARGET_TRIPLET "x64-windows-static")

find_path(D3DX12_INCLUDE_DIRS "d3dx12.h")
include_directories(${D3DX12_INCLUDE_DIRS})

add_subdirectory(Core)
add_subdirectory(Audio)
add_subdirectory(Graphics)
add_subdirectory(GUI)

add_subdirectory(Examples/Pong)

add_compile_options(/wd4996)
