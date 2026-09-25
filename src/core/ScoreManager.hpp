/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** ScoreManager.hpp
*/
#ifndef SCOREMANAGER_HPP_
    #define SCOREMANAGER_HPP_

    #include <string>
    #include <vector>
    #include "../../include/Types.hpp"

class ScoreManager {
    public:
        ScoreManager(const std::string &filePath = "scores.txt");

        void loadScores();
        void saveScores() const;

        void addScore(const Arcade::ScoreEntry &newScore);

        std::vector<Arcade::ScoreEntry> getAllScores() const;
        std::vector<Arcade::ScoreEntry> getScoresForGame(const std::string &gameName) const;
        std::vector<Arcade::ScoreEntry> getTopScoresForGame(const std::string &gameName, std::size_t maxScores) const;

    private:
        std::string _filePath;
        std::vector<Arcade::ScoreEntry> _scores;

        bool parseScoreLine(const std::string &line, Arcade::ScoreEntry &scoreEntry) const;
        std::string scoreToLine(const Arcade::ScoreEntry &scoreEntry) const;

        void sortScores();
};

#endif