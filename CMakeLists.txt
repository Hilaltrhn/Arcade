cmake_minimum_required(VERSION 3.16)
project(arcade LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# ── Core executable
add_subdirectory(src/core)

# ── Graphical libraries
add_subdirectory(src/graphics/sdl2)
add_subdirectory(src/graphics/sfml)
add_subdirectory(src/graphics/ncurse)

# ── Game libraries
add_subdirectory(src/games/snake)
add_subdirectory(src/games/nibbler)
