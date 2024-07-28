cmake_minimum_required(VERSION 3.14)
project(atomGraphics)

set(CMAKE_CXX_STANDARD 23)

include_directories(
        ${CMAKE_SOURCE_DIR}
        ../Shared
)

add_library(atomGraphics STATIC
        ../Shared/Platform.h
        Context.h
        Context.cpp
        Shader.h
        Shader.cpp
)

target_link_libraries(atomGraphics PRIVATE GLEW::GLEW)