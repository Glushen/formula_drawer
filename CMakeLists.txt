cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)

project(formula_drawer_run)

add_executable(formula_drawer_run main.cpp)

include_directories(src)
add_subdirectory(src)

target_link_libraries(formula_drawer_run formula_drawer_lib)
