add_library(arcade_ncurses SHARED
    NcursesDisplay.cpp
    EntryPoint.cpp
)

target_include_directories(arcade_ncurses PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

find_package(Curses REQUIRED)

target_include_directories(arcade_ncurses PRIVATE
    ${CURSES_INCLUDE_DIR}
)

target_link_libraries(arcade_ncurses PRIVATE
    ${CURSES_LIBRARIES}
)

set_target_properties(arcade_ncurses PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
)