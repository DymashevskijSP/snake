cmake_minimum_required(VERSION 3.10)

project(snake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)
add_library(common OBJECT src/game.cpp src/ncurses-view.cpp)
add_executable(snake src/main.cpp)
target_link_libraries(snake common)