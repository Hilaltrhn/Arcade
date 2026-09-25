/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-arcade-11
** File description:
** Sfmldisplay.cpp
*/

#include "SfmlDisplay.hpp"
#include <stdexcept>

namespace Arcade {

SFMLDisplay::SFMLDisplay()
    : _name("SFML")
{
}

SFMLDisplay::~SFMLDisplay()
{
    close();
}

void SFMLDisplay::init()
{
    _window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Arcade - SFML");
    _window.setFramerateLimit(60);

    const char *fontPaths[] = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
        "/usr/share/fonts/TTF/DejaVuSansMono.ttf",
        "/usr/share/fonts/dejavu-sans-mono-fonts/DejaVuSansMono.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf",
        "/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf",
        nullptr
    };

    bool loaded = false;
    for (int i = 0; fontPaths[i] != nullptr; i++) {
        if (_font.loadFromFile(fontPaths[i])) {
            loaded = true;
            break;
        }
    }
    if (!loaded)
        throw std::runtime_error("SFMLDisplay: could not load any TTF font");
}

void SFMLDisplay::close()
{
    if (_window.isOpen())
        _window.close();
}

const std::string &SFMLDisplay::getName() const
{
    return _name;
}

sf::Color SFMLDisplay::toSFMLColor(Color color) const
{
    switch(color) {
        case Color::BLACK:
            return sf::Color::Black;
        case Color::RED:
            return sf::Color::Red;
        case Color::GREEN:
            return sf::Color::Green;
        case Color::YELLOW:
            return sf::Color::Yellow;
        case Color::BLUE:
            return sf::Color::Blue;
        case Color::MAGENTA:
            return sf::Color::Magenta;
        case Color::CYAN:
            return sf::Color::Cyan;
        case Color::WHITE:
            return sf::Color::White;
        default:
            return sf::Color::White;
    }
}

Key SFMLDisplay::mapSFMLKey(sf::Keyboard::Key key) const
{
    static const std::unordered_map<int, Key> keyMap = {
        {sf::Keyboard::Up, Key::UP}, {sf::Keyboard::Down, Key::DOWN},
        {sf::Keyboard::Left, Key::LEFT}, {sf::Keyboard::Right, Key::RIGHT},
        {sf::Keyboard::Return, Key::ENTER}, {sf::Keyboard::BackSpace, Key::BACKSPACE},
        {sf::Keyboard::Space, Key::SPACE}, {sf::Keyboard::Escape, Key::ESCAPE},

        {sf::Keyboard::A, Key::KEY_A}, {sf::Keyboard::B, Key::KEY_B},
        {sf::Keyboard::C, Key::KEY_C}, {sf::Keyboard::D, Key::KEY_D},
        {sf::Keyboard::E, Key::KEY_E}, {sf::Keyboard::F, Key::KEY_F},
        {sf::Keyboard::G, Key::KEY_G}, {sf::Keyboard::H, Key::KEY_H},
        {sf::Keyboard::I, Key::KEY_I}, {sf::Keyboard::J, Key::KEY_J},
        {sf::Keyboard::K, Key::KEY_K}, {sf::Keyboard::L, Key::KEY_L},
        {sf::Keyboard::M, Key::KEY_M}, {sf::Keyboard::N, Key::KEY_N},
        {sf::Keyboard::O, Key::KEY_O}, {sf::Keyboard::P, Key::KEY_P},
        {sf::Keyboard::Q, Key::KEY_Q}, {sf::Keyboard::R, Key::KEY_R},
        {sf::Keyboard::S, Key::KEY_S}, {sf::Keyboard::T, Key::KEY_T},
        {sf::Keyboard::U, Key::KEY_U}, {sf::Keyboard::V, Key::KEY_V},
        {sf::Keyboard::W, Key::KEY_W}, {sf::Keyboard::X, Key::KEY_X},
        {sf::Keyboard::Y, Key::KEY_Y}, {sf::Keyboard::Z, Key::KEY_Z},

        {sf::Keyboard::F1, Key::NEXT_LIB}, {sf::Keyboard::F2, Key::PREV_LIB},
        {sf::Keyboard::F3, Key::NEXT_GAME}, {sf::Keyboard::F4, Key::PREV_GAME},
        {sf::Keyboard::F5, Key::RESTART}, {sf::Keyboard::F6, Key::MENU},
        {sf::Keyboard::F12, Key::QUIT},
    };

    auto it = keyMap.find(static_cast<int>(key));
    return (it != keyMap.end()) ? it->second : Key::NONE;
}

void SFMLDisplay::clear()
{
    _window.clear(sf::Color::Black);
}

void SFMLDisplay::renderCell(int x, int y, const Cell &cell, int cellSize, int offsetX, int offsetY)
{
    float px = offsetX + x * cellSize;
    float py = offsetY + y * cellSize;
    float margin = 1.0f;
    float innerSize = cellSize - margin * 2;
    bool snakeStyle = (cell.bgColor == Color::BLACK);

    if (cell.character == '#') {
        sf::RectangleShape wall(sf::Vector2f(cellSize, cellSize));
        wall.setPosition(px, py);
        if (snakeStyle) {
            wall.setFillColor(sf::Color(42, 26, 62));
            wall.setOutlineThickness(1.0f);
            wall.setOutlineColor(sf::Color(80, 50, 120));
        } else {
            sf::Color c = toSFMLColor(cell.fgColor);
            wall.setFillColor(c);
            wall.setOutlineThickness(1.0f);
            wall.setOutlineColor(sf::Color(c.r, c.g, c.b, 120));
        }
        _window.draw(wall);
        return;
    }

    sf::RectangleShape bg(sf::Vector2f(cellSize, cellSize));
    bg.setPosition(px, py);
    if (snakeStyle) {
        if ((x + y) % 2 == 0)
            bg.setFillColor(sf::Color(10, 10, 18));
        else
            bg.setFillColor(sf::Color(14, 14, 24));
    } else {
        bg.setFillColor(sf::Color(15, 15, 15));
    }
    _window.draw(bg);

    if (cell.character == ' ' || cell.character == '\0')
        return;

    if (cell.character == '@') {
        sf::Color headColor;
        if (cell.fgColor == Color::YELLOW) {
            headColor = sf::Color(0, 255, 255);
        } else {
            headColor = toSFMLColor(cell.fgColor);
        }

        sf::CircleShape glow(innerSize * 0.6f);
        glow.setOrigin(innerSize * 0.6f, innerSize * 0.6f);
        glow.setPosition(px + cellSize / 2.0f, py + cellSize / 2.0f);
        glow.setFillColor(sf::Color(headColor.r, headColor.g, headColor.b, 40));
        _window.draw(glow);

        sf::RectangleShape head(sf::Vector2f(innerSize, innerSize));
        head.setPosition(px + margin, py + margin);
        head.setFillColor(headColor);
        _window.draw(head);

        float eyeR = cellSize / 7.0f;
        float exOff = cellSize / 3.5f;
        float eyOff = cellSize / 3.0f;
        sf::CircleShape eyeL(eyeR);
        eyeL.setOrigin(eyeR, eyeR);
        eyeL.setPosition(px + exOff, py + eyOff);
        eyeL.setFillColor(sf::Color(0, 0, 0));
        _window.draw(eyeL);
        sf::CircleShape eyeR2(eyeR);
        eyeR2.setOrigin(eyeR, eyeR);
        eyeR2.setPosition(px + cellSize - exOff, py + eyOff);
        eyeR2.setFillColor(sf::Color(0, 0, 0));
        _window.draw(eyeR2);
        return;
    }

    if (cell.character == 'o' || cell.character == 'O') {
        sf::Color bodyColor;
        if (cell.fgColor == Color::GREEN) {
            bodyColor = sf::Color(0, 200, 200);
        } else {
            bodyColor = toSFMLColor(cell.fgColor);
        }

        sf::RectangleShape body(sf::Vector2f(innerSize, innerSize));
        body.setPosition(px + margin, py + margin);
        body.setFillColor(bodyColor);
        body.setOutlineThickness(1.0f);
        body.setOutlineColor(sf::Color(bodyColor.r, bodyColor.g, bodyColor.b, 80));
        _window.draw(body);
        return;
    }

    if (cell.character == '*') {
        sf::Color foodColor;
        if (cell.fgColor == Color::RED) {
            foodColor = sf::Color(255, 0, 170);
        } else {
            foodColor = toSFMLColor(cell.fgColor);
        }

        sf::CircleShape glow(innerSize * 0.55f);
        glow.setOrigin(innerSize * 0.55f, innerSize * 0.55f);
        glow.setPosition(px + cellSize / 2.0f, py + cellSize / 2.0f);
        glow.setFillColor(sf::Color(foodColor.r, foodColor.g, foodColor.b, 35));
        _window.draw(glow);

        float foodR = innerSize / 2.0f - 1;
        sf::CircleShape food(foodR);
        food.setOrigin(foodR, foodR);
        food.setPosition(px + cellSize / 2.0f, py + cellSize / 2.0f);
        food.setFillColor(foodColor);
        _window.draw(food);
        return;
    }

    sf::RectangleShape fallback(sf::Vector2f(innerSize, innerSize));
    fallback.setPosition(px + margin, py + margin);
    fallback.setFillColor(toSFMLColor(cell.fgColor));
    _window.draw(fallback);
}

void SFMLDisplay::renderText(const Text &text, int cellSize, int offsetX, int offsetY)
{
    if (text.content.empty())
        return;

    sf::Text sfText;
    sfText.setFont(_font);
    sfText.setString(text.content);
    sfText.setCharacterSize(FONT_SIZE);

    if (text.color == Color::RED)
        sfText.setFillColor(sf::Color(255, 50, 50));
    else
        sfText.setFillColor(sf::Color(180, 200, 220));

    int textY;
    if (text.y >= _lastGridHeight)
        textY = _lastGridBottom + 5 + (text.y - _lastGridHeight) * 22;
    else
        textY = offsetY + text.y * cellSize;

    sfText.setPosition(offsetX, textY);
    _window.draw(sfText);
}

void SFMLDisplay::render(const RenderData &data)
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

void SFMLDisplay::display()
{
    _window.display();
}

Key SFMLDisplay::pollEvent()
{
    sf::Event event;
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            return Key::QUIT;
        if (event.type == sf::Event::KeyPressed)
            return mapSFMLKey(event.key.code);
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left)
                return Key::MOUSE_LEFT;
            if (event.mouseButton.button == sf::Mouse::Right)
                return Key::MOUSE_RIGHT;
        }
    }
    return Key::NONE;
}
}

extern "C" {

    Arcade::IDisplayModule *createDisplayModule()
    {
        return new Arcade::SFMLDisplay();
    }

    void destroyDisplayModule(Arcade::IDisplayModule *module)
    {
        delete module;
    }

}
