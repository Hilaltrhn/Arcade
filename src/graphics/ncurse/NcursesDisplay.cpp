/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** NcursesDisplay.cpp
*/
#include "NcursesDisplay.hpp"
#include <ncurses.h>

NcursesDisplay::NcursesDisplay() {
    _name = "ncurses";
}

void NcursesDisplay::setupNcurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, TRUE);
}

void NcursesDisplay::setupColors() {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
}

void NcursesDisplay::init() {
    setupNcurses();
    setupColors();
}

void NcursesDisplay::close() {
    endwin();
}

const std::string &NcursesDisplay::getName() const {
    return _name;
}

void NcursesDisplay::clear() {
    erase();
}

int NcursesDisplay::convertColor(Arcade::Color color) const {
    if (color == Arcade::Color::RED) {
        return 2;
    }
    if (color == Arcade::Color::GREEN) {
        return 3;
    }
    if (color == Arcade::Color::YELLOW) {
        return 4;
    }
    if (color == Arcade::Color::BLUE) {
        return 5;
    }
    return 1;
}

void NcursesDisplay::renderGrid(const Arcade::RenderData &data) {
    for (int y = 0; y < data.gridHeight; ++y) {
        for (int x = 0; x < data.gridWidth; ++x) {
            const Arcade::Cell &cell = data.grid[y][x];
            int colorPair = convertColor(cell.fgColor);
            attron(COLOR_PAIR(colorPair));
            move(y, x);
            addch(cell.character);
            attroff(COLOR_PAIR(colorPair));
        }
    }
}

void NcursesDisplay::renderTexts(const Arcade::RenderData &data) {
    for (std::size_t index = 0; index < data.texts.size(); ++index) {
        const Arcade::Text &text = data.texts[index];
        int colorPair = convertColor(text.color);
        attron(COLOR_PAIR(colorPair));
        move(text.y, text.x);
        printw("%s", text.content.c_str());
        attroff(COLOR_PAIR(colorPair));
    }
}

void NcursesDisplay::render(const Arcade::RenderData &data) {
    renderGrid(data);
    renderTexts(data);
}

void NcursesDisplay::display() {
    refresh();
}


Arcade::Key NcursesDisplay::mapLetterKeys(int key) const {
    if (key == 'a' || key == 'A')
        return Arcade::Key::KEY_A;
    if (key == 'b' || key == 'B')
        return Arcade::Key::KEY_B;
    if (key == 'c' || key == 'C')
        return Arcade::Key::KEY_C;
    if (key == 'd' || key == 'D')
        return Arcade::Key::KEY_D;
    if (key == 'e' || key == 'E')
        return Arcade::Key::KEY_E;
    if (key == 'f' || key == 'F')
        return Arcade::Key::KEY_F;
    if (key == 'g' || key == 'G')
        return Arcade::Key::KEY_G;
    if (key == 'h' || key == 'H')
        return Arcade::Key::KEY_H;
    if (key == 'i' || key == 'I')
        return Arcade::Key::KEY_I;
    if (key == 'j' || key == 'J')
        return Arcade::Key::KEY_J;
    if (key == 'k' || key == 'K')
        return Arcade::Key::KEY_K;
    if (key == 'l' || key == 'L')
        return Arcade::Key::KEY_L;
    if (key == 'm' || key == 'M')
        return Arcade::Key::KEY_M;
    if (key == 'n' || key == 'N')
        return Arcade::Key::KEY_N;
    if (key == 'o' || key == 'O')
        return Arcade::Key::KEY_O;
    if (key == 'p' || key == 'P')
        return Arcade::Key::KEY_P;
    if (key == 'q' || key == 'Q')
        return Arcade::Key::KEY_Q;
    if (key == 'r' || key == 'R')
        return Arcade::Key::KEY_R;
    if (key == 's' || key == 'S')
        return Arcade::Key::KEY_S;
    if (key == 't' || key == 'T')
        return Arcade::Key::KEY_T;
    if (key == 'u' || key == 'U')
        return Arcade::Key::KEY_U;
    if (key == 'v' || key == 'V')
        return Arcade::Key::KEY_V;
    if (key == 'w' || key == 'W')
        return Arcade::Key::KEY_W;
    if (key == 'x' || key == 'X')
        return Arcade::Key::KEY_X;
    if (key == 'y' || key == 'Y')
        return Arcade::Key::KEY_Y;
    if (key == 'z' || key == 'Z')
        return Arcade::Key::KEY_Z;
    return Arcade::Key::NONE;
}

Arcade::Key NcursesDisplay::pollEvent() {
    int key = getch();

    if (key == ERR)
        return Arcade::Key::NONE;

    if (key == KEY_F(1))
        return Arcade::Key::NEXT_LIB;
    if (key == KEY_F(2))
        return Arcade::Key::PREV_LIB;
    if (key == KEY_F(3))
        return Arcade::Key::NEXT_GAME;
    if (key == KEY_F(4))
        return Arcade::Key::PREV_GAME;
    if (key == KEY_F(5))
        return Arcade::Key::RESTART;
    if (key == KEY_F(6))
        return Arcade::Key::MENU;
    if (key == KEY_F(12))
        return Arcade::Key::QUIT;

    if (key == KEY_UP)
        return Arcade::Key::UP;
    if (key == KEY_DOWN)
        return Arcade::Key::DOWN;
    if (key == KEY_LEFT)
        return Arcade::Key::LEFT;
    if (key == KEY_RIGHT)
        return Arcade::Key::RIGHT;
    if (key == 27)
        return Arcade::Key::ESCAPE;
    if (key == KEY_BACKSPACE || key == 127 || key == 8)
        return Arcade::Key::BACKSPACE;
    if (key == ' ')
        return Arcade::Key::SPACE;

    Arcade::Key letterKey = mapLetterKeys(key);
    if (letterKey != Arcade::Key::NONE)
        return letterKey;

    if (key == 10 || key == KEY_ENTER)
        return Arcade::Key::ENTER;

    return Arcade::Key::NONE;
}