cmake_minimum_required(VERSION 3.14)
project(atomCore)

set(CMAKE_CXX_STANDARD 23)

include_directories(
        ${CMAKE_SOURCE_DIR}
        ../Shared
)

add_library(atomCore STATIC
        ../Shared/Platform.h
        GameApp.cpp
        GameApp.h
        Types.h
        GameWindow.cpp
        GameWindow.h
        Timer.h
        Scene.cpp
        Scene.h
)