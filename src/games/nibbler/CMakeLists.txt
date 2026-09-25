add_library(arcade_nibbler SHARED
    NibblerGame.cpp
)

target_include_directories(arcade_nibbler PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

set_target_properties(arcade_nibbler PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
)