cmake_minimum_required(VERSION 3.14)
project(atomCore)

set(CMAKE_CXX_STANDARD 23)

include_directories(
        ${CMAKE_SOURCE_DIR}
        ../Shared
        ../Graphics
        ../GUI
)

add_library(atomCore STATIC
        ../Shared/Platform.h
        ../Shared/MathUtils.h
        ../Shared/Types.h
        GameApp.cpp
        GameApp.h
        GameWindow.cpp
        GameWindow.h
        Timer.h
        Scene.cpp
        Scene.h
        GameObject.cpp
        GameObject.h
        Color.cpp
        Color.h
        Camera.cpp
        Camera.h
)

target_link_libraries(atomCore PRIVATE atomGUI atomGraphics)