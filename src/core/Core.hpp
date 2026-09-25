/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-arcade-11
** File description:
** Core.hpp
*/

#ifndef CORE_HPP
    #define CORE_HPP

#include "../../include/core/DLLoader.hpp"
#include "../../include/IDisplayModule.hpp"
#include "../../include/IGameModule.hpp"
#include "ScoreManager.hpp"
#include "Menu.hpp"

namespace Arcade {
    class Core {
        public:
            Core(const std::string &initialLibPath);
            ~Core();

            void run();
        private:
            void scanLibraries();
            void handleSystemEvents(Key key);
            void switchGraphical(int direction);
            void switchGame(int direction);
            void launchGame(const std::string &gamePath);

            void processMenuFrame(Key key);
            void processGameFrame(Key key);

            void handleMenuEvents(Key key);
            void handleMenuValidation();

            void updateGameFrame(Key key);
            void handleGameOver();

            void renderMenu();
            void renderGame();

            Arcade::RenderData buildGameRenderDataWithHelp() const;

            void waitForNextFrame(const std::chrono::high_resolution_clock::time_point &startTime);

            std::unique_ptr<DLLoader<IDisplayModule>> _graphicalLoader;
            std::unique_ptr<DLLoader<IGameModule>> _gameLoader;

            IDisplayModule *_graphicalModule;
            IGameModule *_gameModule;

            std::vector<std::string> _graphicalLibs;
            std::vector<std::string> _gameLibs;

            bool _running;
            bool _inMenu;
            size_t _currentGraphIndex = 0;
            size_t _currentGameIndex = 0;

            std::string _playerName;
            Menu _menu;
            ScoreManager _scoreManager;
            bool _scoreSaved = false;
    };
}

#endif
