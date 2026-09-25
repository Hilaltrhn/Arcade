/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** test_ncurses.cpp
*/
#include "../src/graphics/ncurse/NcursesDisplay.hpp"
#include "../src/games/snake/SnakeGame.hpp"

#include <thread>
#include <chrono>

int main()
{
    NcursesDisplay display;
    SnakeGame game;

    display.init();
    game.init();

    bool running = true;

    while (running) {
        Arcade::Key key = display.pollEvent();

        if (key == Arcade::Key::QUIT || key == Arcade::Key::ESCAPE) {
            running = false;
        } else if (key == Arcade::Key::RESTART) {
            game.reset();
        } else {
            game.handleInput(key);
        }

        game.update();

        display.clear();
        display.render(game.getRenderData());
        display.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    display.close();
    return 0;
}