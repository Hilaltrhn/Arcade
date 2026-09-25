/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-arcade-11
** File description:
** Sdl2display.cpp
*/

#include "Sdl2Display.hpp"
#include <stdexcept>
#include <iostream>
#include <unordered_map>

namespace Arcade {

SDL2Display::SDL2Display()
    : _name("SDL2"), _window(nullptr), _renderer(nullptr), _font(nullptr)
{
}

SDL2Display::~SDL2Display()
{
    close();
}

void SDL2Display::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        throw std::runtime_error(std::string("SDL_Init: ") + SDL_GetError());

    if (TTF_Init() != 0)
        throw std::runtime_error(std::string("TTF_Init: ") + TTF_GetError());

    _window = SDL_CreateWindow("Arcade - SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!_window)
        throw std::runtime_error(std::string("SDL_CreateWindow: ") + SDL_GetError());

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!_renderer)
        throw std::runtime_error(std::string("SDL_CreateRenderer: ") + SDL_GetError());

    const char *fontPaths[] = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
        "/usr/share/fonts/TTF/DejaVuSansMono.ttf",
        "/usr/share/fonts/dejavu-sans-mono-fonts/DejaVuSansMono.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf",
        "/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf",
        nullptr
    };

    for (int i = 0; fontPaths[i] != nullptr; i++) {
        _font = TTF_OpenFont(fontPaths[i], FONT_SIZE);
        if (_font)
            break;
    }
    if (!_font)
        throw std::runtime_error("SDL2Display: could not laod any TTF font");
}

void SDL2Display::close()
{
    if (_font) {
        TTF_CloseFont(_font);
        _font = nullptr;
    }
    if (_renderer) {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
    if (_window) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
    TTF_Quit();
    SDL_Quit();
}

const std::string &SDL2Display::getName() const
{
    return _name;
}

SDL_Color SDL2Display::toSDLColor(Color color) const
{
    switch (color) {
        case Color::BLACK:
            return {0, 0, 0, 255};
        case Color::RED:
            return {255, 0, 0, 255};
        case Color::GREEN:
            return {0, 200, 0, 255};
        case Color::YELLOW:
            return {255, 255, 0, 255};
        case Color::BLUE:
            return {0, 0, 255, 255};
        case Color::MAGENTA:
            return {255, 0, 0, 255};
        case Color::CYAN:
            return {0, 255, 255, 255};
        case Color::WHITE:
            return {255, 255, 255, 255};
        default:
            return {255, 255, 255, 255};
    }
}

Key SDL2Display::mapSDLKey(SDL_Keycode key) const
{
    static const std::unordered_map<SDL_Keycode, Key> keyMap = {
        {SDLK_UP, Key::UP}, {SDLK_DOWN, Key::DOWN}, {SDLK_LEFT, Key::LEFT},
        {SDLK_RIGHT, Key::RIGHT}, {SDLK_RETURN, Key::ENTER}, {SDLK_BACKSPACE, Key::BACKSPACE},
        {SDLK_SPACE, Key::SPACE}, {SDLK_ESCAPE, Key::ESCAPE}, {SDLK_a, Key::KEY_A},
        {SDLK_b, Key::KEY_B}, {SDLK_c, Key::KEY_C}, {SDLK_d, Key::KEY_D},
        {SDLK_e, Key::KEY_E}, {SDLK_f, Key::KEY_F}, {SDLK_g, Key::KEY_G},
        {SDLK_h, Key::KEY_H}, {SDLK_i, Key::KEY_I}, {SDLK_j, Key::KEY_J},
        {SDLK_k, Key::KEY_K}, {SDLK_l, Key::KEY_L}, {SDLK_m, Key::KEY_M},
        {SDLK_n, Key::KEY_N}, {SDLK_o, Key::KEY_O}, {SDLK_p, Key::KEY_P},
        {SDLK_q, Key::KEY_Q}, {SDLK_r, Key::KEY_R}, {SDLK_s, Key::KEY_S},
        {SDLK_t, Key::KEY_T}, {SDLK_u, Key::KEY_U}, {SDLK_v, Key::KEY_V},
        {SDLK_w, Key::KEY_W}, {SDLK_x, Key::KEY_X}, {SDLK_y, Key::KEY_Y},
        {SDLK_z, Key::KEY_Z}, {SDLK_F1, Key::NEXT_LIB}, {SDLK_F2, Key::PREV_LIB},
        {SDLK_F3, Key::NEXT_GAME}, {SDLK_F4, Key::PREV_GAME}, {SDLK_F5, Key::RESTART},
        {SDLK_F6, Key::MENU}, {SDLK_F12, Key::QUIT},
    };
    auto it = keyMap.find(key);
    return (it != keyMap.end()) ? it->second : Key::NONE;
}

void SDL2Display::renderCell(int x, int y, const Cell &cell, int cellSize, int offsetX, int offsetY)
{
    int px = offsetX + x * cellSize;
    int py = offsetY + y * cellSize;
    bool isGame = (cellSize > 20);

    // le mur
    if (cell.character == '#') {
        if (cell.fgColor == Color::BLUE) {
            SDL_SetRenderDrawColor(_renderer, 87, 138, 52, 255);
        } else {
            SDL_Color c = toSDLColor(cell.fgColor);
            SDL_SetRenderDrawColor(_renderer, c.r, c.g, c.b, c.a);
        }
        SDL_Rect rect = {px, py, cellSize, cellSize};
        SDL_RenderFillRect(_renderer, &rect);
        return;
    }

    // le background slon le jeu
    if (cell.bgColor == Color::BLACK) {
        if ((x + y) % 2 == 0)
            SDL_SetRenderDrawColor(_renderer, 170, 215, 81, 255);
        else
            SDL_SetRenderDrawColor(_renderer, 162, 209, 73, 255);
    } else {
        SDL_SetRenderDrawColor(_renderer, 15, 15, 15, 255);
    }
    SDL_Rect bgRect = {px, py, cellSize, cellSize};
    SDL_RenderFillRect(_renderer, &bgRect);

    if (cell.character == ' ' || cell.character == '\0')
        return;

    // la tete du serpent
    if (cell.character == '@') {
        if (cell.fgColor == Color::YELLOW) {
            SDL_SetRenderDrawColor(_renderer, 70, 116, 233, 255);
        } else {
            SDL_Color c = toSDLColor(cell.fgColor);
            SDL_SetRenderDrawColor(_renderer, c.r, c.g, c.b, c.a);
        }
        SDL_Rect head = {px + 1, py + 1, cellSize - 2, cellSize - 2};
        SDL_RenderFillRect(_renderer, &head);

        int eyeSize = std::max(2, cellSize / 5);
        int eyeY = py + cellSize / 4;
        int eyeLeftX = px + cellSize / 4;
        int eyeRightX = px + cellSize - cellSize / 4 - eyeSize;
        SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
        SDL_Rect eyeL = {eyeLeftX, eyeY, eyeSize, eyeSize};
        SDL_Rect eyeR = {eyeRightX, eyeY, eyeSize, eyeSize};
        SDL_RenderFillRect(_renderer, &eyeL);
        SDL_RenderFillRect(_renderer, &eyeR);

        int pupilSize = std::max(1, eyeSize / 2);
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
        SDL_Rect pupilL = {eyeLeftX + 1, eyeY + 1, pupilSize, pupilSize};
        SDL_Rect pupilR = {eyeRightX + 1, eyeY + 1, pupilSize, pupilSize};
        SDL_RenderFillRect(_renderer, &pupilL);
        SDL_RenderFillRect(_renderer, &pupilR);
        return;
    }

    // le corps du serpent
    if (cell.character == 'o' || cell.character == 'O') {
        if (cell.fgColor == Color::GREEN) {
            SDL_SetRenderDrawColor(_renderer, 60, 100, 209, 255);
        } else {
            SDL_Color c = toSDLColor(cell.fgColor);
            SDL_SetRenderDrawColor(_renderer, c.r, c.g, c.b, c.a);
        }
        SDL_Rect body = {px + 1, py + 1, cellSize - 2, cellSize - 2};
        SDL_RenderFillRect(_renderer, &body);
        return;
    }

    // la pomme
    if (cell.character == '*') {
        if (cell.fgColor == Color::RED) {
            SDL_SetRenderDrawColor(_renderer, 231, 71, 29, 255);
        } else {
            SDL_Color c = toSDLColor(cell.fgColor);
            SDL_SetRenderDrawColor(_renderer, c.r, c.g, c.b, c.a);
        }
        SDL_Rect food = {px + 2, py + 2, cellSize - 4, cellSize - 4};
        SDL_RenderFillRect(_renderer, &food);

        if (cell.fgColor == Color::RED) {
            SDL_SetRenderDrawColor(_renderer, 67, 138, 37, 255);
            SDL_Rect stem = {px + cellSize / 2 - 1, py, 2, 3};
            SDL_RenderFillRect(_renderer, &stem);
        }
        return;
    }

    // fallback
    SDL_Color fg = toSDLColor(cell.fgColor);
    SDL_SetRenderDrawColor(_renderer, fg.r, fg.g, fg.b, fg.a);
    SDL_Rect fallback = {px + 1, py + 1, cellSize - 2, cellSize - 2};
    SDL_RenderFillRect(_renderer, &fallback);
}

void SDL2Display::clear()
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
}

void SDL2Display::renderText(const Text &text, int cellSize, int offsetX, int offsetY)
{
    if (text.content.empty())
        return;

    SDL_Color fg = {220, 220, 220, 255};
    if (text.color == Color::RED)
        fg = {255, 50, 50, 255};

    SDL_Surface *surface = TTF_RenderText_Blended(_font, text.content.c_str(), fg);
    if (!surface)
        return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface);
    if (texture) {
        int textY;
        if (text.y >= _lastGridHeight)
            textY = _lastGridBottom + 5 + (text.y - _lastGridHeight) * 22;
        else
            textY = offsetY + text.y * cellSize;

        SDL_Rect dst = {
            offsetX,
            textY,
            surface->w,
            surface->h
        };
        SDL_RenderCopy(_renderer, texture, nullptr, &dst);
        SDL_DestroyTexture(texture);
    }
    SDL_FreeSurface(surface);
}

void SDL2Display::render(const RenderData &data)
{
    if (data.gridWidth == 0 || data.gridHeight == 0)
        return;

    int cellW = WINDOW_WIDTH / data.gridWidth;
    int textSpace = 120;
    int cellH = (WINDOW_HEIGHT - textSpace) / data.gridHeight;
    int cellSize = std::min(cellW, cellH);
    int offsetX = (WINDOW_WIDTH - cellSize * data.gridWidth) / 2;
    int offsetY = 10;

    _lastGridBottom = offsetY + data.gridHeight * cellSize;
    _lastGridHeight = data.gridHeight;

    for (int y = 0; y < data.gridHeight && y < (int)data.grid.size(); y++) {
        for (int x = 0; x < data.gridWidth && x < (int)data.grid[y].size(); x++) {
            renderCell(x, y, data.grid[y][x], cellSize, offsetX, offsetY);
        }
    }
    for (const auto &text : data.texts)
        renderText(text, cellSize, offsetX, offsetY);
}

void SDL2Display::display()
{
    SDL_RenderPresent(_renderer);
}

Key SDL2Display::pollEvent()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return Key::QUIT;
        if (event.type == SDL_KEYDOWN)
            return mapSDLKey(event.key.keysym.sym);
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT)
                return Key::MOUSE_LEFT;
            if (event.button.button == SDL_BUTTON_RIGHT)
                return Key::MOUSE_RIGHT;
        }
    }
    return Key::NONE;
}
}

extern "C" {

    Arcade::IDisplayModule *createDisplayModule()
    {
        return new Arcade::SDL2Display();
    }

    void destroyDisplayModule(Arcade::IDisplayModule * module)
    {
        delete module;
    }
}

