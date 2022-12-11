cmake_minimum_required(VERSION 3.0.0)
project(Basic VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(Basic main.cpp)

find_package(SDL2 REQUIRED COMPONENTS SDL2)

target_link_libraries(Basic SDL2::SDL2)


