/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** SnakeGame.hpp
*/
#ifndef SNAKEGAME_HPP
    #define SNAKEGAME_HPP

    #include <deque>
    #include <string>

    #include "../../../include/IGameModule.hpp"
    #include "../../../include/Types.hpp"

class SnakeGame : public Arcade::IGameModule {
    public:
        SnakeGame();
        ~SnakeGame() override = default;

        void init() override;
        void reset() override;
        void close() override;
        const std::string &getName() const override;

        void handleInput(Arcade::Key key) override;

        const Arcade::RenderData &getRenderData() override;
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
        Arcade::RenderData _renderData;
        std::deque<Position> _snakeBody;
        Position _foodPosition;
        Direction _currentDirection;
        Direction _nextDirection;
        bool _gameOver;
        int _score;
        int _gridWidth;
        int _gridHeight;

        void initializeGrid();
        void initializeSnake();
        void spawnFood();
        bool isPositionOnSnake(int x, int y) const;

        void drawWalls();
        void drawFood();
        void drawSnake();
        void addGameTexts();
        void setCell(int x, int y, char character, Arcade::Color color);
        void buildRenderData();

        bool isOppositeDirection(Direction firstDirection, Direction secondDirection) const;

        Position getNextHeadPosition() const;
};


#endif