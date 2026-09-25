/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-arcade-11
** File description:
** Sdl2display.hpp
*/

#ifndef SDL2DISPLAY_HPP
#define SDL2DISPLAY_HPP

#include "../../../include/IDisplayModule.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <unordered_map>

namespace Arcade {
    class SDL2Display : public IDisplayModule {
    public:
        SDL2Display();
        ~SDL2Display() override;
        void init() override;
        void close() override;
        const std::string &getName() const override;
        void clear() override;
        void render(const RenderData &data) override;
        void display() override;
        Key pollEvent() override;

    private:
        SDL_Color toSDLColor(Color color) const;
        Key mapSDLKey(SDL_Keycode key) const;
        void renderCell(int x, int y, const Cell &cell, int cellSize, int offsetX, int offsetY);
        void renderText(const Text &text, int cellSize, int offsetX, int offsetY);
        std::string _name;
        SDL_Window *_window;
        SDL_Renderer *_renderer;
        TTF_Font *_font;
        static constexpr int WINDOW_WIDTH = 800;
        static constexpr int WINDOW_HEIGHT = 600;
        static constexpr int FONT_SIZE = 16;
        int _lastGridBottom = 0;
        int _lastGridHeight = 0;
    };
}

#endif