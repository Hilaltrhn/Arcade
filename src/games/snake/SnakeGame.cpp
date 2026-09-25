/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** SnakeGame.cpp
*/
#include "SnakeGame.hpp"
#include <cstdlib>
#include <ctime>

SnakeGame::SnakeGame() {
    _gameName = "Snake";
    _gridWidth = 20;
    _gridHeight = 20;
    _gameOver = false;
    _score = 0;
    _currentDirection = RIGHT;
    _nextDirection = RIGHT;
    std::srand(std::time(nullptr));
}

const std::string &SnakeGame::getName() const {
    return _gameName;
}

void SnakeGame::initializeGrid() {
    _renderData.gridWidth = _gridWidth;
    _renderData.gridHeight = _gridHeight;
    _renderData.grid.clear();
    _renderData.texts.clear();
    for (int y = 0; y < _gridHeight; ++y) {
        std::vector<Arcade::Cell> row;
        for (int x = 0; x < _gridWidth; ++x) {
            Arcade::Cell cell;
            cell.character = ' ';
            cell.fgColor = Arcade::Color::WHITE;
            cell.bgColor = Arcade::Color::BLACK;
            row.push_back(cell);
        }
        _renderData.grid.push_back(row);
    }
}

void SnakeGame::initializeSnake() {
    _snakeBody.clear();
    int centerX = _gridWidth / 2;
    int centerY = _gridHeight / 2;
    Position head;
    head.x = centerX;
    head.y = centerY;
    Position bodyPart1;
    bodyPart1.x = centerX - 1;
    bodyPart1.y = centerY;
    Position bodyPart2;
    bodyPart2.x = centerX - 2;
    bodyPart2.y = centerY;
    Position bodyPart3;
    bodyPart3.x = centerX - 3;
    bodyPart3.y = centerY;
    _snakeBody.push_back(head);
    _snakeBody.push_back(bodyPart1);
    _snakeBody.push_back(bodyPart2);
    _snakeBody.push_back(bodyPart3);
}

bool SnakeGame::isPositionOnSnake(int x, int y) const {
    for (std::size_t index = 0; index < _snakeBody.size(); ++index) {
        int snakeX = _snakeBody[index].x;
        int snakeY = _snakeBody[index].y;
        if (snakeX == x && snakeY == y) {
            return true;
        }
    }
    return false;
}

void SnakeGame::spawnFood() {
    int randomX = 1 + (std::rand() % (_gridWidth - 2));
    int randomY = 1 + (std::rand() % (_gridHeight - 2));
    while (isPositionOnSnake(randomX, randomY)) {
        randomX = 1 + (std::rand() % (_gridWidth - 2));
        randomY = 1 + (std::rand() % (_gridHeight - 2));
    }
    _foodPosition.x = randomX;
    _foodPosition.y = randomY;
}

void SnakeGame::setCell(int x, int y, char character, Arcade::Color color) {
    _renderData.grid[y][x].character = character;
    _renderData.grid[y][x].fgColor = color;
}

void SnakeGame::drawWalls() {
    for (int x = 0; x < _gridWidth; ++x) {
        setCell(x, 0, '#', Arcade::Color::BLUE);
        setCell(x, _gridHeight - 1, '#', Arcade::Color::BLUE);
    }
    for (int y = 0; y < _gridHeight; ++y) {
        setCell(0, y, '#', Arcade::Color::BLUE);
        setCell(_gridWidth - 1, y, '#', Arcade::Color::BLUE);
    }
}

void SnakeGame::drawFood() {
    setCell(_foodPosition.x, _foodPosition.y, '*', Arcade::Color::RED);
}

void SnakeGame::drawSnake() {
    for (std::size_t index = 0; index < _snakeBody.size(); ++index) {
        int x = _snakeBody[index].x;
        int y = _snakeBody[index].y;
        if (index == 0) {
            setCell(x, y, '@', Arcade::Color::YELLOW);
        } else {
            setCell(x, y, 'o', Arcade::Color::GREEN);
        }
    }
}

void SnakeGame::addGameTexts() {
    Arcade::Text gameTitleText;
    gameTitleText.content = "Snake";
    gameTitleText.x = 0;
    gameTitleText.y = _gridHeight + 1;
    gameTitleText.color = Arcade::Color::WHITE;
    _renderData.texts.push_back(gameTitleText);
    Arcade::Text scoreText;
    scoreText.content = "Score: " + std::to_string(_score);
    scoreText.x = 0;
    scoreText.y = _gridHeight + 2;
    scoreText.color = Arcade::Color::WHITE;
    _renderData.texts.push_back(scoreText);
    if (_gameOver) {
        Arcade::Text gameOverText;
        gameOverText.content = "Game Over - Press Restart";
        gameOverText.x = 0;
        gameOverText.y = _gridHeight + 3;
        gameOverText.color = Arcade::Color::RED;
        _renderData.texts.push_back(gameOverText);
    }
}

void SnakeGame::buildRenderData() {
    initializeGrid();
    drawWalls();
    drawFood();
    drawSnake();
    addGameTexts();
}



void SnakeGame::init() {
    _gameOver = false;
    _score = 0;
    _currentDirection = RIGHT;
    _nextDirection = RIGHT;
    initializeGrid();
    initializeSnake();
    spawnFood();
    buildRenderData();
}

void SnakeGame::reset() {
    init();
}

void SnakeGame::close() {

}


bool SnakeGame::isOppositeDirection(Direction firstDirection, Direction secondDirection) const {
    if (firstDirection == UP && secondDirection == DOWN) {
        return true;
    }
    if (firstDirection == DOWN && secondDirection == UP) {
        return true;
    }
    if (firstDirection == LEFT && secondDirection == RIGHT) {
        return true;
    }
    if (firstDirection == RIGHT && secondDirection == LEFT) {
        return true;
    }
    return false;
}

void SnakeGame::handleInput(Arcade::Key key) {
    Direction wantedDirection = _currentDirection;
    bool validKey = true;
    if (key == Arcade::Key::UP) {
        wantedDirection = UP;
    } else if (key == Arcade::Key::DOWN) {
        wantedDirection = DOWN;
    } else if (key == Arcade::Key::LEFT) {
        wantedDirection = LEFT;
    } else if (key == Arcade::Key::RIGHT) {
        wantedDirection = RIGHT;
    } else {
        validKey = false;
    }
    if (!validKey) {
        return;
    }
    if (!isOppositeDirection(_currentDirection, wantedDirection)) {
        _nextDirection = wantedDirection;
    }
}

const Arcade::RenderData &SnakeGame::getRenderData() {
    return _renderData;
}

int SnakeGame::getScore() const {
    return _score;
}

bool SnakeGame::isGameOver() const {
    return _gameOver;
}

SnakeGame::Position SnakeGame::getNextHeadPosition() const {
    Position nextHeadPosition = _snakeBody.front();
    if (_currentDirection == UP) {
        nextHeadPosition.y -= 1;
    } else if (_currentDirection == DOWN) {
        nextHeadPosition.y += 1;
    } else if (_currentDirection == LEFT) {
        nextHeadPosition.x -= 1;
    } else if (_currentDirection == RIGHT) {
        nextHeadPosition.x += 1;
    }
    return nextHeadPosition;
}

void SnakeGame::update() {
    if (_gameOver) {
        buildRenderData();
        return;
    }
    _currentDirection = _nextDirection;
    Position nextHeadPosition = getNextHeadPosition();
    if (nextHeadPosition.x <= 0 || nextHeadPosition.x >= _gridWidth - 1 ||
        nextHeadPosition.y <= 0 || nextHeadPosition.y >= _gridHeight - 1) {
        _gameOver = true;
        buildRenderData();
        return;
    }
    if (isPositionOnSnake(nextHeadPosition.x, nextHeadPosition.y)) {
        _gameOver = true;
        buildRenderData();
        return;
    }
    _snakeBody.push_front(nextHeadPosition);
    if (nextHeadPosition.x == _foodPosition.x && nextHeadPosition.y == _foodPosition.y) {
        _score += 1;
        spawnFood();
    } else {
        _snakeBody.pop_back();
    }
    buildRenderData();
}