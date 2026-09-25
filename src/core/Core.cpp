/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-arcade-11
** File description:
** Core.cpp
*/
#include "Core.hpp"

Arcade::Core::Core(const std::string &initialLibPath)
    : _graphicalModule(nullptr), _gameModule(nullptr), _running(true), _inMenu(true), _scoreManager("scores.txt") {
    scanLibraries();
    _scoreManager.loadScores();
    _menu.setAvailableGames(_gameLibs);
    _menu.setAvailableDisplays(_graphicalLibs);
    _menu.setScores(_scoreManager.getAllScores());
    auto it = std::find(_graphicalLibs.begin(), _graphicalLibs.end(), initialLibPath);
    if (it != _graphicalLibs.end()) {
        _currentGraphIndex = std::distance(_graphicalLibs.begin(), it);
    }
    _graphicalLoader = std::make_unique<DLLoader<IDisplayModule>>(initialLibPath);
    _graphicalModule = _graphicalLoader->getInstance({ "createDisplayModule", "createDisplay", "entryPoint" });
}

Arcade::Core::~Core() {
    if (_graphicalModule) {
        _graphicalModule->close();
        delete _graphicalModule;
    }
    if (_gameModule) {
        _gameModule->close();
        delete _gameModule;
    }
}

void Arcade::Core::processMenuFrame(Arcade::Key key) {
    handleMenuEvents(key);
    if (!_running) {
        return;
    }
    _menu.handleInput(key);
    if (_menu.isMenuFinished()) {
        handleMenuValidation();
    }
    if (_inMenu) {
        renderMenu();
    }
}

void Arcade::Core::handleMenuEvents(Arcade::Key key) {
    if (key == Key::QUIT) {
        _running = false;
    }
    if (key == Key::ESCAPE) {
        _running = false;
    }
}

void Arcade::Core::handleMenuValidation() {
    _playerName = _menu.getPlayerName();
    std::string selectedDisplayPath = _menu.getSelectedDisplay();
    std::string selectedGamePath = _menu.getSelectedGame();
    auto displayIt = std::find(_graphicalLibs.begin(), _graphicalLibs.end(), selectedDisplayPath);
    if (displayIt != _graphicalLibs.end()) {
        _currentGraphIndex = std::distance(_graphicalLibs.begin(), displayIt);
        _graphicalModule->close();
        delete _graphicalModule;
        _graphicalLoader.reset();
        _graphicalLoader = std::make_unique<DLLoader<IDisplayModule>>(_graphicalLibs[_currentGraphIndex]);
        _graphicalModule = _graphicalLoader->getInstance({ "createDisplayModule", "createDisplay", "entryPoint", "entryPointDisplay" });
        _graphicalModule->init();
    }
    auto gameIt = std::find(_gameLibs.begin(), _gameLibs.end(), selectedGamePath);
    if (gameIt != _gameLibs.end()) {
        _currentGameIndex = std::distance(_gameLibs.begin(), gameIt);
    }
    launchGame(selectedGamePath);
    _inMenu = false;
}

void Arcade::Core::renderMenu() {
    _graphicalModule->clear();
    _graphicalModule->render(_menu.getRenderData());
    _graphicalModule->display();
}

void Arcade::Core::processGameFrame(Arcade::Key key) {
    handleSystemEvents(key);
    if (!_running || _inMenu) {
        return;
    }
    updateGameFrame(key);
    renderGame();
}

void Arcade::Core::updateGameFrame(Arcade::Key key) {
    if (!_gameModule) {
        return;
    }
    _gameModule->handleInput(key);
    _gameModule->update();
    if (_gameModule->isGameOver()) {
        if (!_scoreSaved) {
            _scoreManager.addScore({_playerName, _gameModule->getName(), _gameModule->getScore()});
            _scoreSaved = true;
        }
        if (key == Key::RESTART) {
            _gameModule->reset();
            _scoreSaved = false;
        }
        if (key == Key::ESCAPE) {
            _inMenu = true;
            _scoreSaved = false;
            _menu.resetMenuState();
        }
    }
}

void Arcade::Core::handleGameOver() {
    _scoreManager.addScore({_playerName, _gameModule->getName(), _gameModule->getScore()});
    _menu.setScores(_scoreManager.getAllScores());
    _inMenu = true;
    _menu.resetMenuState();
}

Arcade::RenderData Arcade::Core::buildGameRenderDataWithHelp() const {
    Arcade::RenderData data = _gameModule->getRenderData();
    int yOffset = 0;
    for (const auto &text : data.texts) {
        if (text.y > yOffset)
            yOffset = text.y;
    }
    Arcade::Text instructions;
    instructions.content = "F1/F2: lib | F3/F4: game | F5: restart | F6: menu | F12: quit";
    instructions.x = 1;
    instructions.y = yOffset + 1;
    instructions.color = Arcade::Color::WHITE;
    data.texts.push_back(instructions);
    return data;
}

void Arcade::Core::renderGame() {
    _graphicalModule->clear();
    if (_gameModule) {
        Arcade::RenderData data = buildGameRenderDataWithHelp();
        _graphicalModule->render(data);
    }
    _graphicalModule->display();
}

void Arcade::Core::waitForNextFrame(const std::chrono::high_resolution_clock::time_point &startTime) {
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = endTime - startTime;
    if (elapsed.count() < 100) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100) - std::chrono::milliseconds((int)elapsed.count()));
    }
}

void Arcade::Core::run() {
    _graphicalModule->init();
    while (_running) {
        auto startTime = std::chrono::high_resolution_clock::now();
        Key key = _graphicalModule->pollEvent();
        if (_inMenu) {
            processMenuFrame(key);
        } else {
            processGameFrame(key);
        }
        waitForNextFrame(startTime);
    }
}

void Arcade::Core::scanLibraries() {
    _graphicalLibs.clear();
    _gameLibs.clear();
    if (!std::filesystem::exists("./lib")) {
        throw std::runtime_error("Directory ./lib/ not found");
    }
    for (const auto &entry : std::filesystem::directory_iterator("./lib")) {
        std::string path = entry.path().string();
        if (path.size() >= 3 && path.substr(path.size() - 3) == ".so") {
            if (path.find("ncurses") != std::string::npos ||
                path.find("sdl2") != std::string::npos ||
                path.find("sfml") != std::string::npos) {
                _graphicalLibs.push_back(path);
            } else {
                _gameLibs.push_back(path);
            }
        }
    }
    std::sort(_graphicalLibs.begin(), _graphicalLibs.end());
    std::sort(_gameLibs.begin(), _gameLibs.end());
}

void Arcade::Core::handleSystemEvents(Key key) {
    switch (key) {
        case Key::QUIT:
            _running = false;
            break;
        case Key::ESCAPE:
        case Key::MENU:
            _inMenu = true;
            _menu.resetMenuState();
            break;
        case Key::NEXT_LIB:
            switchGraphical(1);
            break;
        case Key::PREV_LIB:
            switchGraphical(-1);
            break;
        case Key::NEXT_GAME:
            switchGame(1);
            break;
        case Key::PREV_GAME:
            switchGame(-1);
            break;
        case Key::RESTART:
            if (_gameModule)
                _gameModule->reset();
            break;
        default:
            break;
    }
}

void Arcade::Core::launchGame(const std::string &gamePath) {
    if (_gameModule) {
        _gameModule->close();
        delete _gameModule;
    }
    _scoreSaved = false;
    _gameLoader.reset();
    _gameLoader = std::make_unique<DLLoader<IGameModule>>(gamePath);
    _gameModule = _gameLoader->getInstance({ "createGameModule", "createGame", "entryPoint", "entryPointGame" });
    _gameModule->init();
}

void Arcade::Core::switchGraphical(int direction) {
    if (_graphicalLibs.empty()) {
        return;
    }
    _currentGraphIndex = (_currentGraphIndex + direction + _graphicalLibs.size()) % _graphicalLibs.size();
    _graphicalModule->close();
    delete _graphicalModule;
    _graphicalLoader.reset();
    _graphicalLoader = std::make_unique<DLLoader<IDisplayModule>>(_graphicalLibs[_currentGraphIndex]);
    _graphicalModule = _graphicalLoader->getInstance({ "createDisplayModule", "createDisplay", "entryPoint", "entryPointDisplay" });
    _graphicalModule->init();
}

void Arcade::Core::switchGame(int direction) {
    if (_gameLibs.empty()) {
        return;
    }
    _currentGameIndex = (_currentGameIndex + direction + _gameLibs.size()) % _gameLibs.size();
    launchGame(_gameLibs[_currentGameIndex]);
}
