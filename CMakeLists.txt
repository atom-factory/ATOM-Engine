cmake_minimum_required(VERSION 3.14)
project(ATOM)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(Core)
add_subdirectory(Audio)
add_subdirectory(Examples)