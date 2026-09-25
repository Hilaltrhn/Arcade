/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** EntryPoint.cpp
*/

#include "SnakeGame.hpp"

extern "C" Arcade::IGameModule *createGameModule(){
    return new SnakeGame();
}

extern "C" void destroyGameModule(Arcade::IGameModule *gameModule) {
    delete gameModule;
}
