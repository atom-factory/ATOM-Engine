cmake_minimum_required(VERSION 3.14)
project(atomExamples)

set(CMAKE_CXX_STANDARD 23)

include_directories(
        ../Core
        ../Shared
)

add_executable(ExampleGameApp WIN32 ExampleGameApp.cpp)
target_link_libraries(ExampleGameApp PRIVATE atomCore)