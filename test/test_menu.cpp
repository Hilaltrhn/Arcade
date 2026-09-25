/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** test_menu.cpp
*/
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <iostream>

#include "../src/core/Menu.hpp"
#include "../src/core/ScoreManager.hpp"
#include "../src/graphics/ncurse/NcursesDisplay.hpp"

int main()
{
    Menu menu;
    NcursesDisplay display;
    ScoreManager scoreManager("scores.txt");

    std::vector<std::string> availableGames;
    availableGames.push_back("Snake");
    availableGames.push_back("Nibbler");

    std::vector<std::string> availableDisplays;
    availableDisplays.push_back("ncurses");
    availableDisplays.push_back("SDL2");

    scoreManager.loadScores();

    menu.setAvailableGames(availableGames);
    menu.setAvailableDisplays(availableDisplays);
    menu.setScores(scoreManager.getAllScores());

    display.init();

    bool running = true;

    while (running) {
        Arcade::Key key = display.pollEvent();

        if (key == Arcade::Key::ESCAPE) {
            running = false;
        } else {
            menu.handleInput(key);
        }

        display.clear();
        display.render(menu.getRenderData());
        display.display();

        if (menu.isMenuFinished()) {
            display.close();

            std::cout << "Menu finished" << std::endl;
            std::cout << "Selected game: " << menu.getSelectedGame() << std::endl;
            std::cout << "Selected display: " << menu.getSelectedDisplay() << std::endl;
            std::cout << "Player name: " << menu.getPlayerName() << std::endl;

            return 0;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    display.close();
    return 0;
}