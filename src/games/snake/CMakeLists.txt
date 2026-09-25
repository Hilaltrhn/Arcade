add_library(arcade_snake SHARED
    SnakeGame.cpp
    EntryPoint.cpp
)

target_include_directories(arcade_snake PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

set_target_properties(arcade_snake PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
)