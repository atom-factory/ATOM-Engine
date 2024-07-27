cmake_minimum_required(VERSION 3.14)
project(atomCore)

set(CMAKE_CXX_STANDARD 23)

include_directories(
        ../Lib
)

add_executable(ExampleAudio WIN32 ExampleAudio.cpp)
target_link_libraries(ExampleAudio PRIVATE atomCore)

add_executable(ExampleGameApp WIN32 ExampleGameApp.cpp)
target_link_libraries(ExampleGameApp PRIVATE atomCore)