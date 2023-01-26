cmake_minimum_required(VERSION 3.0.0)
project(raycaster VERSION 0.1.0)


find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

message(STATUS "SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2_INCLUDE_DIRS: ${SDL2_LIBRARIES}")

find_library(SDL2_LIB SDL2)

add_definitions(-w)

add_executable(raycaster main.cpp ./src/kiss_sdl/kiss_widgets ./src/kiss_sdl/kiss_general ./src/kiss_sdl/kiss_draw ./src/kiss_sdl/kiss_posix.c ./src/kiss_sdl/kiss_sdl.h)
target_link_libraries(raycaster ${SDL2_LIBRARIES} SDL2_ttf SDL2_image)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY bin DESTINATION ${CMAKE_BINARY_DIR})
file(COPY src DESTINATION ${CMAKE_BINARY_DIR})

