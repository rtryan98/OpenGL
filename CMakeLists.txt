add_subdirectory(vendor)

cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(NAME OpenglExamples)

project(${NAME}
    VERSION 1.0
    DESCRIPTION "OpenGL 4.6 Examples"
    )

add_subdirectory(base)
add_subdirectory(examples)
