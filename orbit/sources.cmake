get_filename_component(library_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(objects_name ${library_name}_object)

set(library_src
    include/${library_name}/centerofmass.h
    include/${library_name}/orbital_elements.h
    include/${library_name}/position.h
    include/${library_name}/universal_kepler_solver.h
)

set(object_library_src
    src/centerofmass.cpp
    src/orbit_helper.cpp
    src/orbit_helper.h
    src/orbital_elements.cpp
    src/position.cpp

    src/keplersolver/degenerate.h
    src/keplersolver/elliptic.h
    src/keplersolver/hyperbolic.h
    src/keplersolver/parabolic.h
    src/keplersolver/solver.cpp
    src/keplersolver/solver.h
)

# Trick to show the file sources.cmake in the IDE
list(APPEND library_src sources.cmake)
set_source_files_properties(sources.cmake PROPERTIES HEADER_FILE_ONLY TRUE)
