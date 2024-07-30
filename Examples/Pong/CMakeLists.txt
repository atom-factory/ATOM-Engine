cmake_minimum_required(VERSION 3.14)
project(atomExamples)

set(CMAKE_CXX_STANDARD 23)

include_directories(
        ../../Core
        ../../Shared
        ../../Audio
        ../../Graphics
        ../../GUI
)

add_executable(Pong WIN32
        main.cpp
        Ball.cpp
        Ball.h
)

target_link_libraries(Pong PRIVATE
        atomCore
        atomAudio
        atomGraphics
        atomGUI
)

add_custom_command(TARGET Pong PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)