cmake_minimum_required(VERSION 3.0.0)
project(Basic VERSION 0.1.0)

include(CTest)

enable_testing()
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2TTF_INCLUDE_DIR})

find_package(SDL2 REQUIRED)
message ([STATUS] " Found SDL: " ${SDL2_FOUND})
message ([STATUS] " SDL2_INCLUDE_DIR | " ${SDL2_INCLUDE_DIRS})
message ([STATUS] " SDL2_LIBRARY | " ${SDL2_LIBRARIES})
message ([STATUS] " SDL2TFF_LIBRARY | " ${SDL2TTF_LIBRARY})

find_package(SDL2TTF REQUIRED)

find_library(SDL2_LIBRARY Basic SDL2)

add_executable(Basic main.cpp)

include_directories(MyProject ${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIR})
target_link_libraries(Basic ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARY})





