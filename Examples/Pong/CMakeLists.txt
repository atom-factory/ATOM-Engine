cmake_minimum_required(VERSION 3.14)
project(atomExamples)

set(CMAKE_CXX_STANDARD 23)

include_directories(
        ../../Core
        ../../Shared
        ../../Audio
        ../../Graphics
)

add_executable(Pong WIN32 main.cpp
        Ball.cpp
        Ball.h)
target_link_libraries(Pong PRIVATE atomCore atomAudio atomGraphics GLEW::GLEW)