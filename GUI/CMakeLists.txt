cmake_minimum_required(VERSION 3.14)
project(atomGUI)

set(CMAKE_CXX_STANDARD 23)

include_directories(
        ${CMAKE_SOURCE_DIR}
        ../Shared
        ../Core
)

add_library(atomGUI STATIC
        ../Shared/Platform.h
        ../Shared/IO.h
        ../Shared/IO.cpp
        Text.h
        Text.cpp
        Font.cpp
        Font.h
)

target_link_libraries(atomGUI PRIVATE atomCore)