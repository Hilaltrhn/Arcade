add_library(arcade_sfml SHARED
    SfmlDisplay.cpp
)

target_include_directories(arcade_sfml PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

find_package(SFML 2 COMPONENTS graphics window system REQUIRED)

target_link_libraries(arcade_sfml PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

set_target_properties(arcade_sfml PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
)