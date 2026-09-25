/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** Menu.cpp
*/
#include "Menu.hpp"

Menu::Menu() {
    _selectedGameIndex = 0;
    _selectedDisplayIndex = 0;
    _playerName = "";
    _menuFinished = false;
}

void Menu::setAvailableGames(const std::vector<std::string> &games) {
    _games = games;
    buildRenderData();
}

void Menu::setAvailableDisplays(const std::vector<std::string> &displays) {
    _displays = displays;
    buildRenderData();
}

void Menu::handleGameSelectionInput(Arcade::Key key) {
    if (key == Arcade::Key::UP) {
        if (_selectedGameIndex > 0) {
            _selectedGameIndex--;
        }
    }
    if (key == Arcade::Key::DOWN) {
        if (_selectedGameIndex < (int)_games.size() - 1) {
            _selectedGameIndex++;
        }
    }
}

void Menu::handleDisplaySelectionInput(Arcade::Key key) {
    if (key == Arcade::Key::LEFT) {
        if (_selectedDisplayIndex > 0) {
            _selectedDisplayIndex--;
        }
    }
    if (key == Arcade::Key::RIGHT) {
        if (_selectedDisplayIndex < (int)_displays.size() - 1) {
            _selectedDisplayIndex++;
        }
    }
}

void Menu::handlePlayerNameInput(Arcade::Key key) {
    if (key == Arcade::Key::BACKSPACE) {
        if (!_playerName.empty()) {
            _playerName.pop_back();
        }
        return;
    }
    char character = keyToCharacter(key);
    if (character != '\0') {
        _playerName += character;
    }
}

char Menu::keyToCharacter(Arcade::Key key) const {
    if (key >= Arcade::Key::KEY_A && key <= Arcade::Key::KEY_Z) {
        return 'A' + (static_cast<int>(key) - static_cast<int>(Arcade::Key::KEY_A));
    }
    return '\0';
}

void Menu::handleInput(Arcade::Key key) {
    if (key == Arcade::Key::ENTER) {
        _menuFinished = true;
        buildRenderData();
        return;
    }
    handleGameSelectionInput(key);
    handleDisplaySelectionInput(key);
    handlePlayerNameInput(key);
    buildRenderData();
}

const Arcade::RenderData &Menu::getRenderData() {
    return _renderData;
}

std::string Menu::getSelectedGame() const {
    if (_games.empty()) return "";
    return _games[_selectedGameIndex];
}

std::string Menu::getSelectedDisplay() const {
    if (_displays.empty()) return "";
    return _displays[_selectedDisplayIndex];
}

std::string Menu::getPlayerName() const {
    return _playerName;
}

void Menu::initializeGrid() {
    _renderData.gridWidth = 40;
    _renderData.gridHeight = 23;
    _renderData.grid.clear();
    _renderData.texts.clear();
    for (int y = 0; y < _renderData.gridHeight; ++y) {
        std::vector<Arcade::Cell> row;
        for (int x = 0; x < _renderData.gridWidth; ++x) {
            Arcade::Cell cell;
            cell.character = ' ';
            cell.fgColor = Arcade::Color::WHITE;
            cell.bgColor = Arcade::Color::WHITE;
            row.push_back(cell);
        }
        _renderData.grid.push_back(row);
    }
}

void Menu::addTitleText() {
    Arcade::Text title;
    title.content = "=== ARCADE MENU ===";
    title.x = 1;
    title.y = 1;
    title.color = Arcade::Color::YELLOW;
    _renderData.texts.push_back(title);
}

void Menu::addGamesTexts() {
    for (std::size_t index = 0; index < _games.size(); ++index) {
        Arcade::Text text;
        text.content = _games[index];
        text.x = 2;
        text.y = 3 + index;
        if ((int)index == _selectedGameIndex) {
            text.content = "> " + text.content;
            text.color = Arcade::Color::GREEN;
        } else {
            text.color = Arcade::Color::WHITE;
        }
        _renderData.texts.push_back(text);
    }
}

void Menu::addDisplaysTexts() {
    for (std::size_t index = 0; index < _displays.size(); ++index) {
        Arcade::Text text;
        text.content = _displays[index];
        text.x = 5;
        text.y = 5 + index;
        if ((int)index == _selectedDisplayIndex) {
            text.content = "> " + text.content;
            text.color = Arcade::Color::CYAN;
        } else {
            text.color = Arcade::Color::WHITE;
        }
        _renderData.texts.push_back(text);
    }
}

void Menu::addPlayerNameText() {
    Arcade::Text player;
    player.content = "Player: " + _playerName;
    player.x = 2;
    player.y = 18;
    player.color = Arcade::Color::WHITE;
    _renderData.texts.push_back(player);
}

void Menu::setScores(const std::vector<Arcade::ScoreEntry> &scores) {
    _scores = scores;
    buildRenderData();
}

static std::string extractGameNameFromLibPath(const std::string &gamePath) {
    std::string fileName = gamePath;
    std::size_t slashPos = fileName.find_last_of("/\\");
    if (slashPos != std::string::npos)
        fileName = fileName.substr(slashPos + 1);
    if (fileName.rfind("arcade_", 0) == 0)
        fileName = fileName.substr(7);
    std::size_t dotPos = fileName.rfind(".so");
    if (dotPos != std::string::npos)
        fileName = fileName.substr(0, dotPos);
    if (!fileName.empty())
        fileName[0] = static_cast<char>(std::toupper(fileName[0]));
    for (std::size_t i = 1; i < fileName.size(); ++i)
        fileName[i] = static_cast<char>(std::tolower(fileName[i]));
    return fileName;
}

std::vector<Arcade::ScoreEntry> Menu::getScoresForSelectedGame() const {
    std::vector<Arcade::ScoreEntry> selectedGameScores;
    std::string selectedGame = extractGameNameFromLibPath(getSelectedGame());
    for (std::size_t index = 0; index < _scores.size(); ++index) {
        if (_scores[index].gameName == selectedGame) {
            selectedGameScores.push_back(_scores[index]);
        }
    }
    return selectedGameScores;
}

void Menu::addScoresTexts() {
    Arcade::Text scoreTitle;
    scoreTitle.content = "Top Scores";
    scoreTitle.x = 2;
    scoreTitle.y = 10;
    scoreTitle.color = Arcade::Color::YELLOW;
    _renderData.texts.push_back(scoreTitle);
    std::vector<Arcade::ScoreEntry> selectedGameScores = getScoresForSelectedGame();
    if (selectedGameScores.empty()) {
        Arcade::Text noScoreText;
        noScoreText.content = "No score yet";
        noScoreText.x = 2;
        noScoreText.y = 11;
        noScoreText.color = Arcade::Color::WHITE;
        _renderData.texts.push_back(noScoreText);
        return;
    }
    int maxScoresToShow = 5;
    if ((int)selectedGameScores.size() < maxScoresToShow) {
        maxScoresToShow = selectedGameScores.size();
    }
    for (int index = 0; index < maxScoresToShow; ++index) {
        Arcade::Text scoreText;
        scoreText.content = selectedGameScores[index].playerName + " - " +
            std::to_string(selectedGameScores[index].score);
        scoreText.x = 2;
        scoreText.y = 11 + index;
        scoreText.color = Arcade::Color::WHITE;
        _renderData.texts.push_back(scoreText);
    }
}

bool Menu::isMenuFinished() const {
    return _menuFinished;
}

void Menu::resetMenuState() {
    _menuFinished = false;
    buildRenderData();
}

void Menu::addInstructionsText() {
    Arcade::Text instructions;
    instructions.content = "UP/DOWN: game | LEFT/RIGHT: lib | ENTER: play | F12: quit";
    instructions.x = 2;
    instructions.y = 20;
    instructions.color = Arcade::Color::WHITE;
    _renderData.texts.push_back(instructions);
    Arcade::Text playerHelp;
    playerHelp.content = "Type your name with A-Z | BACKSPACE: erase";
    playerHelp.x = 2;
    playerHelp.y = 21;
    playerHelp.color = Arcade::Color::WHITE;
    _renderData.texts.push_back(playerHelp);
}

void Menu::buildRenderData() {
    initializeGrid();
    addTitleText();
    addGamesTexts();
    addDisplaysTexts();
    addScoresTexts();
    addPlayerNameText();
    addInstructionsText();
}