include(sources.cmake REQUIRED)

add_library(${library_name} SHARED ${library_src})

source_group("res" REGULAR_EXPRESSION ".*")
source_group("src" REGULAR_EXPRESSION ".*\\.(cpp|h|inl)")
source_group("include" REGULAR_EXPRESSION "include/${library_name}/.*")

generate_export_header(${library_name} EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/${library_name}_export.h)

target_include_directories(${library_name}
PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${library_name}/include>
    $<INSTALL_INTERFACE:include>
)

add_subdirectory(test)
