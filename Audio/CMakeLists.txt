cmake_minimum_required(VERSION 3.14)
project(atomAudio)

set(CMAKE_CXX_STANDARD 23)

include_directories(
        ${CMAKE_SOURCE_DIR}
        ../Shared
)

add_library(atomAudio STATIC
        ../Shared/Platform.h
        Audio.h
        Audio.cpp
)