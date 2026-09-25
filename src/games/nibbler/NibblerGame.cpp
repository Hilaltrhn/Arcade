/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-arcade-11
** File description:
** Nibblergame.cpp
*/

#include "NibblerGame.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
namespace Arcade {

NibblerGame::NibblerGame()
{
    _gameName = "Nibbler";
    _gridWidth = 19;
    _gridHeight = 17;
    _gameOver = false;
    _score = 0;
    _currentLevel = 0;
    _currentDirection = UP;
    _nextDirection = UP;
    _tickCount = 0;
    _tickSpeed = 5;
    std::srand(std::time(nullptr));
}

const std::string &NibblerGame::getName() const
{
    return _gameName;
}

void NibblerGame::loadLevelFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        return;

    _maze.clear();
    _snakeBody.clear();
    _foodPositions.clear();
    _currentDirection = UP;
    _nextDirection = UP;
    _tickCount = 0;

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty())
            lines.push_back(line);
    }
    file.close();

    _gridHeight = lines.size();
    _gridWidth = lines[0].size();

    int startX = 0;
    int startY = 0;

    for (int y = 0; y < _gridHeight; ++y) {
        std::vector<int> row;
        for (int x = 0; x < _gridWidth && x < (int)lines[y].size(); ++x) {
            char c = lines[y][x];
            if (c == '#') {
                row.push_back(1);
            } else {
                row.push_back(0);
                if (c == 'S') {
                    startX = x;
                    startY = y;
                }
            }
        }
        _maze.push_back(row);
    }

    initializeSnake(startX, startY);
    spawnFood(15 + _currentLevel * 5);
}

void NibblerGame::loadLevel(int level)
{
    std::string path = "src/games/nibbler/levels/level" + std::to_string(level + 1) + ".txt";
    loadLevelFromFile(path);

    if (_maze.empty()) {
        _currentLevel = 0;
        loadLevelFromFile("src/games/nibbler/levels/level1.txt");
    }
}

void NibblerGame::initializeGrid()
{
    _renderData.gridWidth = _gridWidth;
    _renderData.gridHeight = _gridHeight;
    _renderData.grid.clear();
    _renderData.texts.clear();
    for (int y = 0; y < _gridHeight; ++y) {
        std::vector<Cell> row;
        for (int x = 0; x < _gridWidth; ++x) {
            Cell cell;
            cell.character = ' ';
            cell.fgColor = Color::WHITE;
            cell.bgColor = Color::WHITE;
            row.push_back(cell);
        }
        _renderData.grid.push_back(row);
    }
}

bool NibblerGame::isPositionOnSnake(int x, int y) const
{
    for (std::size_t i = 0; i < _snakeBody.size(); ++i) {
        if (_snakeBody[i].x == x && _snakeBody[i].y == y) {
            return true;
        }
    }
    return false;
}

bool NibblerGame::isPositionFood(int x, int y) const
{
    for (std::size_t i = 0; i < _foodPositions.size(); ++i) {
        if (_foodPositions[i].x == x && _foodPositions[i].y == y) {
            return true;
        }
    }
    return false;
}

bool NibblerGame::isWall(int x, int y) const
{
    if (x < 0 || x >= _gridWidth || y < 0 || y >= _gridHeight)
        return true;
    return _maze[y][x] == 1;
}

bool NibblerGame::isFree(int x, int y) const
{
    return !isWall(x, y);
}

bool NibblerGame::isOppositeDirection(Direction first, Direction second) const
{
    if (first == UP && second == DOWN)
        return true;
    if (first == DOWN && second == UP)
        return true;
    if (first == LEFT && second == RIGHT)
        return true;
    if (first == RIGHT && second == LEFT)
        return true;
    return false;
}

NibblerGame::Position NibblerGame::getNextPosition(int x, int y, Direction dir) const
{
    Position next;
    next.x = x;
    next.y = y;

    if (dir == UP)
        next.y -= 1;
    else if (dir == DOWN)
        next.y += 1;
    else if (dir == LEFT)
        next.x -= 1;
    else if (dir == RIGHT)
        next.x += 1;
    return next;
}

void NibblerGame::initializeSnake(int startX, int startY)
{
    _snakeBody.clear();
    for (int i = 0; i < 4; ++i) {
        Position part;
        part.x = startX;
        part.y = startY - i;
        _snakeBody.push_back(part);
    }
}

void NibblerGame::spawnFood(int count)
{
    std::vector<Position> freeCells;
    for (int y = 0; y < _gridHeight; ++y) {
        for (int x = 0; x < _gridWidth; ++x) {
            if (isFree(x, y) && !isPositionOnSnake(x, y) && !isPositionFood(x, y)) {
                Position pos;
                pos.x = x;
                pos.y = y;
                freeCells.push_back(pos);
            }
        }
    }
    for (int i = 0; i < count && !freeCells.empty(); ++i) {
        int idx = std::rand() % freeCells.size();
        _foodPositions.push_back(freeCells[idx]);
        freeCells.erase(freeCells.begin() + idx);
    }
}

NibblerGame::Direction NibblerGame::autoTurn(Direction current, int headX, int headY) const
{
    Position ahead = getNextPosition(headX, headY, current);
    if (!isWall(ahead.x, ahead.y) && !isPositionOnSnake(ahead.x, ahead.y))
        return current;

    Direction leftDir, rightDir, backDir;
    if (current == UP) {
        leftDir = LEFT;
        rightDir = RIGHT;
        backDir = DOWN;
    } else if (current == DOWN) {
        leftDir = RIGHT;
        rightDir = LEFT;
        backDir = UP;
    } else if (current == LEFT) {
        leftDir = DOWN;
        rightDir = UP;
        backDir = RIGHT;
    } else {
        leftDir = UP;
        rightDir = DOWN;
        backDir = LEFT;
    }

    Position leftPos = getNextPosition(headX, headY, leftDir);
    Position rightPos = getNextPosition(headX, headY, rightDir);
    Position backPos = getNextPosition(headX, headY, backDir);

    bool canLeft = !isWall(leftPos.x, leftPos.y) && !isPositionOnSnake(leftPos.x, leftPos.y);
    bool canRight = !isWall(rightPos.x, rightPos.y) && !isPositionOnSnake(rightPos.x, rightPos.y);
    bool canBack = !isWall(backPos.x, backPos.y) && !isPositionOnSnake(backPos.x, backPos.y);

    if (canLeft)
        return leftDir;
    if (canRight)
        return rightDir;
    if (canBack)
        return backDir;
    return current;
}

void NibblerGame::setCell(int x, int y, char character, Color color)
{
    if (y < 0 || y >= _gridHeight || x < 0 || x >= _gridWidth)
        return;
    _renderData.grid[y][x].character = character;
    _renderData.grid[y][x].fgColor = color;
}

void NibblerGame::drawMaze()
{
    if (_maze.empty())
        return;
    for (int y = 0; y < _gridHeight; ++y) {
        for (int x = 0; x < _gridWidth; ++x) {
            if (_maze[y][x] == 1) {
                setCell(x, y, '#', Color::CYAN);
            }
        }
    }
}

void NibblerGame::drawFood()
{
    for (std::size_t i = 0; i < _foodPositions.size(); ++i) {
        setCell(_foodPositions[i].x, _foodPositions[i].y, '*', Color::YELLOW);
    }
}

void NibblerGame::drawSnake()
{
    for (std::size_t i = 0; i < _snakeBody.size(); ++i) {
        int x = _snakeBody[i].x;
        int y = _snakeBody[i].y;
        if (i == 0) {
            setCell(x, y, '@', Color::RED);
        } else {
            setCell(x, y, 'o', Color::RED);
        }
    }
}

void NibblerGame::addGameTexts()
{
    Text gameTitleText;
    gameTitleText.content = "Nibbler";
    gameTitleText.x = 0;
    gameTitleText.y = _gridHeight;
    gameTitleText.color = Color::WHITE;
    _renderData.texts.push_back(gameTitleText);

    Text scoreText;
    scoreText.content = "Score: " + std::to_string(_score);
    scoreText.x = 0;
    scoreText.y = _gridHeight + 1;
    scoreText.color = Color::WHITE;
    _renderData.texts.push_back(scoreText);

    Text levelText;
    levelText.content = "Level: " + std::to_string(_currentLevel + 1) + "  Food: " + std::to_string(_foodPositions.size());
    levelText.x = 0;
    levelText.y = _gridHeight + 2;
    levelText.color = Color::CYAN;
    _renderData.texts.push_back(levelText);

    if (_gameOver) {
        Text gameOverText;
        gameOverText.content = "Game Over - Press Restart";
        gameOverText.x = 0;
        gameOverText.y = _gridHeight + 4;
        gameOverText.color = Color::RED;
        _renderData.texts.push_back(gameOverText);
    }
}

void NibblerGame::buildRenderData()
{
    initializeGrid();
    drawMaze();
    drawFood();
    drawSnake();
    addGameTexts();
}

void NibblerGame::init()
{
    _gameOver = false;
    _score = 0;
    _currentLevel = 0;
    _tickCount = 0;
    _tickSpeed = 5;
    loadLevel(_currentLevel);
    buildRenderData();
}

void NibblerGame::reset()
{
    init();
}

void NibblerGame::close()
{
}

void NibblerGame::handleInput(Arcade::Key key)
{
    Direction wantedDirection = _currentDirection;
    bool validKey = true;
    if (key == Key::UP) {
        wantedDirection = UP;
    } else if (key == Key::DOWN) {
        wantedDirection = DOWN;
    } else if (key == Key::LEFT) {
        wantedDirection = LEFT;
    } else if (key == Key::RIGHT) {
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

void NibblerGame::update()
{
    if (_gameOver) {
        buildRenderData();
        return;
    }

    _tickCount++;
    if (_tickCount < _tickSpeed) {
        buildRenderData();
        return;
    }
    _tickCount = 0;

    if (_foodPositions.empty()) {
        _currentLevel++;
        _tickSpeed = std::max(2, _tickSpeed - 1);
        loadLevel(_currentLevel);
        buildRenderData();
        return;
    }

    _currentDirection = _nextDirection;
    Position testPos = getNextPosition(_snakeBody.front().x, _snakeBody.front().y, _currentDirection);
    if (isWall(testPos.x, testPos.y) || isPositionOnSnake(testPos.x, testPos.y))
        _currentDirection = autoTurn(_currentDirection, _snakeBody.front().x, _snakeBody.front().y);

    Position nextHead = getNextPosition(_snakeBody.front().x, _snakeBody.front().y, _currentDirection);
    if (isWall(nextHead.x, nextHead.y) || isPositionOnSnake(nextHead.x, nextHead.y)) {
        _gameOver = true;
        buildRenderData();
        return;
    }

    if (isPositionOnSnake(nextHead.x, nextHead.y)) {
        _gameOver = true;
        buildRenderData();
        return;
    }
    _snakeBody.push_front(nextHead);

    bool ate = false;
    for (std::size_t i = 0; i < _foodPositions.size(); ++i) {
        if (_foodPositions[i].x == nextHead.x && _foodPositions[i].y == nextHead.y) {
            _foodPositions.erase(_foodPositions.begin() + i);
            _score += 10;
            ate = true;
            break;
        }
    }

    if (!ate) {
        _snakeBody.pop_back();
    }
    buildRenderData();
}

const Arcade::RenderData &NibblerGame::getRenderData()
{
    return _renderData;
}

int NibblerGame::getScore() const
{
    return _score;
}

bool NibblerGame::isGameOver() const
{
    return _gameOver;
}

}

extern "C" Arcade::IGameModule *createGameModule()
{
    return new Arcade::NibblerGame();
}

extern "C" void destroyGameModule(Arcade::IGameModule *gameModule)
{
    delete gameModule;
}