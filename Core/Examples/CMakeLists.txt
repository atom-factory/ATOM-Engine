cmake_minimum_required(VERSION 3.14)
project(atomCore)

set(CMAKE_CXX_STANDARD 23)

include_directories(
        ../Lib
)

add_executable(ExampleAudio ExampleAudio.cpp)
target_link_libraries(ExampleAudio PRIVATE atomCore)