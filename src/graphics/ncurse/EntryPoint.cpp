/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** EntryPoint.cpp
*/
#include "NcursesDisplay.hpp"

extern "C" Arcade::IDisplayModule *createDisplayModule() {
    return new NcursesDisplay();
}

extern "C" void destroyDisplayModule(Arcade::IDisplayModule *module) {
    delete module;
}