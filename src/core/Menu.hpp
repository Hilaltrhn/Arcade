/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** Menu.hpp
*/
#ifndef MENU_HPP_
    #define MENU_HPP_

    #include <string>
    #include <vector>
    #include <algorithm>
    #include <cctype>

    #include "../../include/Types.hpp"
    #include "ScoreManager.hpp"

class Menu {
    public:
        Menu();

        void setAvailableGames(const std::vector<std::string> &games);
        void setAvailableDisplays(const std::vector<std::string> &displays);
        void setScores(const std::vector<Arcade::ScoreEntry> &scores);

        void handleInput(Arcade::Key key);

        const Arcade::RenderData &getRenderData();

        std::string getSelectedGame() const;
        std::string getSelectedDisplay() const;
        std::string getPlayerName() const;

        bool isMenuFinished() const;
        void resetMenuState();

        void addInstructionsText();

    private:
        Arcade::RenderData _renderData;

        std::vector<std::string> _games;
        std::vector<std::string> _displays;
        std::vector<Arcade::ScoreEntry> _scores;

        int _selectedGameIndex;
        int _selectedDisplayIndex;

        std::string _playerName;
        bool _menuFinished;

        void buildRenderData();

        void handleGameSelectionInput(Arcade::Key key);
        void handleDisplaySelectionInput(Arcade::Key key);
        void handlePlayerNameInput(Arcade::Key key);
        char keyToCharacter(Arcade::Key key) const;

        void initializeGrid();
        void addTitleText();
        void addGamesTexts();
        void addDisplaysTexts();
        void addPlayerNameText();
        void addScoresTexts();

        std::vector<Arcade::ScoreEntry> getScoresForSelectedGame() const;
};

#endif