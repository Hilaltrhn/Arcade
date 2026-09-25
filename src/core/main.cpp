/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-arcade-11
** File description:
** main.cpp
*/

#include "../../include/core/DLLoader.hpp"
#include "../../include/IDisplayModule.hpp"
#include "Core.hpp"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: ./arcade [path_to_graphical_lib.so]" << std::endl;
        return 84;
    }

    try {
        Arcade::Core core(argv[1]);
        core.run();
    } catch (const std::exception &e) {
        std::cerr << "Critical Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
