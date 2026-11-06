cmake_minimum_required(VERSION 3.18)

project(orbitographie)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    -g
    -pedantic
    -Wall
    -Werror=switch
    -Wextra
    -Wswitch
    -fPIC
)

include(GenerateExportHeader)

find_package(Catch2 REQUIRED)
find_package(Eigen3 REQUIRED)

add_subdirectory(core)
add_subdirectory(math)
add_subdirectory(orbit)
