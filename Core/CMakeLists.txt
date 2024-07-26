cmake_minimum_required(VERSION 3.14)
project(atomCore)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(
        ${CMAKE_SOURCE_DIR}/Lib
)

add_library(atomCore STATIC
        Lib/Audio.cpp
        Lib/Audio.h
        Lib/Platform.h
        Lib/GameApp.cpp
        Lib/GameApp.h
)

# Examples
add_subdirectory(Examples)