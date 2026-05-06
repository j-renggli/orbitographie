cmake_minimum_required(VERSION 3.18)

project(orbitographie)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    -g
    -pedantic
    -Wall
    -Wextra
    -fPIC
)

include(GenerateExportHeader)

find_package(Catch2 REQUIRED)
find_package(Eigen3 REQUIRED)

add_subdirectory(core)
add_subdirectory(math)
add_subdirectory(orbit)

set(library_src
    main.cpp
    .clang-format
)

set_source_files_properties(.clang-format PROPERTIES HEADER_FILE_ONLY TRUE)

add_executable(orbitographie ${library_src})

set_property(TARGET orbitographie PROPERTY COMPILE_WARNING_AS_ERROR ON)

target_link_libraries(orbitographie
    math
    orbit
)

source_group("res" REGULAR_EXPRESSION ".*")
source_group("src" REGULAR_EXPRESSION ".*\\.(cpp|h|inl)")
