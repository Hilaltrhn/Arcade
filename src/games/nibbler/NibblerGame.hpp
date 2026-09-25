/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-arcade-11
** File description:
** NibblerGame.hpp
*/

#ifndef NIBBLERGAME_HPP
#define NIBBLERGAME_HPP

    #include <deque>
    #include <string>
    #include <vector>
    #include "../../../include/IGameModule.hpp"
    #include "../../../include/Types.hpp"
namespace Arcade {
class NibblerGame : public IGameModule {
    public:
        NibblerGame();
        ~NibblerGame() override = default;
        void init() override;
        void reset() override;
        void close() override;
        const std::string &getName() const override;
        void handleInput(Key key) override;
        const RenderData &getRenderData() override;
        int getScore() const override;
        bool isGameOver() const override;
        void update() override;

    private:
        struct Position {
            int x;
            int y;
        };

        enum Direction {
            UP,
            DOWN,
            LEFT,
            RIGHT
        };

        std::string _gameName;
        RenderData _renderData;
        std::deque<Position> _snakeBody;
        std::vector<Position> _foodPositions;
        Direction _currentDirection;
        Direction _nextDirection;
        bool _gameOver;
        int _score;
        int _gridWidth;
        int _gridHeight;
        int _currentLevel;
        int _tickCount;
        int _tickSpeed;
        void loadLevelFromFile(const std::string &filename);
        std::vector<std::vector<int>> _maze;
        void initializeSnake(int startX, int startY);
        void spawnFood(int count);
        void loadLevel(int level);
        void initializeGrid();
        bool isPositionOnSnake(int x, int y) const;
        bool isPositionFood(int x, int y) const;
        bool isWall(int x, int y) const;
        bool isFree(int x, int y) const;
        bool isOppositeDirection(Direction first, Direction second) const;
        Position getNextPosition(int x, int y, Direction dir) const;
        Direction autoTurn(Direction current, int headX, int headY) const;
        void drawMaze();
        void drawFood();
        void drawSnake();
        void setCell(int x, int y, char character, Color color);
        void buildRenderData();
        void addGameTexts();
    };
}

#endif