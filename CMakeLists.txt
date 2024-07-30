cmake_minimum_required(VERSION 3.14)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(VCPKG_TARGET_TRIPLET "x64-windows-static")

project(ATOM)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(DIRECTX_DXC_TOOL "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows-static/tools/directx-dxc/dxc.exe")

find_path(D3DX12_INCLUDE_DIRS "d3dx12.h")
include_directories(${D3DX12_INCLUDE_DIRS})

add_subdirectory(${CMAKE_SOURCE_DIR}/DirectXTK12 ${CMAKE_BINARY_DIR}/bin/CMake/DirectXTK12)

add_subdirectory(Core)
add_subdirectory(Audio)
add_subdirectory(Graphics)
add_subdirectory(GUI)

add_subdirectory(Examples/Pong)

add_compile_options(/wd4996)
