cmake_minimum_required(VERSION 3.16)

project(RoutesApp)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

qt_add_executable(RoutesApp
    Main.cpp
)

target_link_libraries(RoutesApp PRIVATE
    Qt6::Widgets
)