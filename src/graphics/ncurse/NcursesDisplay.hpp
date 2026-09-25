/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** NcursesDisplay.hpp
*/
#ifndef NCURSESDISPLAY_HPP_
    #define NCURSESDISPLAY_HPP_

    #include "../../../include/IDisplayModule.hpp"
    #include "../../../include/Types.hpp"

    #include <string>

class NcursesDisplay : public Arcade::IDisplayModule {
    public:
        NcursesDisplay();
        ~NcursesDisplay() override = default;

        void init() override;
        void close() override;

        const std::string &getName() const override;

        void clear() override;
        void render(const Arcade::RenderData &data) override;
        void display() override;

        Arcade::Key pollEvent() override;

    private:
        std::string _name;
        int convertColor(Arcade::Color color) const;
        void setupNcurses();
        void setupColors();

        void renderGrid(const Arcade::RenderData &data);
        void renderTexts(const Arcade::RenderData &data);

        Arcade::Key mapLetterKeys(int key) const;
};

#endif