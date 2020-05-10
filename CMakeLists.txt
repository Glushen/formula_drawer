cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)

project(formula_drawer)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

qt5_add_resources(formula_drawer_resources res/resources.qrc)
add_executable(formula_drawer main.cpp ${formula_drawer_resources})

include_directories(src)
add_subdirectory(src)

target_link_libraries(formula_drawer formula_drawer_lib)
