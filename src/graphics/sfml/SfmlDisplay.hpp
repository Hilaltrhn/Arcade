/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-arcade-11
** File description:
** Sfmldisplay.hpp
*/

#ifndef SFMLDISPLAY_HPP
#define SFMLDISPLAY_HPP

#include "../../../include/IDisplayModule.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

namespace Arcade {

    class SFMLDisplay : public IDisplayModule {
        public:
        SFMLDisplay();
        ~SFMLDisplay() override;

        void init() override;
        void close() override;
        const std::string &getName() const override;
        void clear() override;
        void render(const RenderData &data) override;
        void display() override;
        Key pollEvent() override;

        private:
        sf::Color toSFMLColor(Color color) const;
        Key mapSFMLKey(sf::Keyboard::Key key) const;
        void renderCell(int x, int y, const Cell &cell, int cellSize, int offsetX, int offsetY);
        void renderText(const Text &text, int cellSize, int offsetX, int offsetY);
        std::string _name;
        sf::RenderWindow _window;
        sf::Font _font;
        static constexpr int WINDOW_WIDTH = 800;
        static constexpr int WINDOW_HEIGHT = 600;
        static constexpr int FONT_SIZE = 14;
        int _lastGridBottom = 0;
        int _lastGridHeight = 0;

    };
}

#endif