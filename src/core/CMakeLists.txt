# Liste des sources pour le Core
set(CORE_SOURCES
    main.cpp
    Core.cpp
    Menu.cpp
    ScoreManager.cpp
)

# On force la sortie à la racine du projet
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(arcade ${CORE_SOURCES})

# Liens nécessaires
target_link_libraries(arcade PRIVATE ${CMAKE_DL_LIBS})

# Inclusion des headers
target_include_directories(arcade PRIVATE .)