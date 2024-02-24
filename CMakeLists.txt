cmake_minimum_required(VERSION 3.21)
include("graphics/cmake/project-setup.cmake")
set(ProjectName "dsg2024")
project(DSG-2024-Proj VERSION 1.0)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(graphics)
add_subdirectory(src)
