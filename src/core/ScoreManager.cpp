/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** ScoreManager.cpp
*/
#include "ScoreManager.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

ScoreManager::ScoreManager(const std::string &filePath)
    : _filePath(filePath) {
}

bool ScoreManager::parseScoreLine(const std::string &line, Arcade::ScoreEntry &scoreEntry) const {
    std::stringstream lineStream(line);
    std::string scoreValue;
    if (!std::getline(lineStream, scoreEntry.playerName, ';')) {
        return false;
    }
    if (!std::getline(lineStream, scoreEntry.gameName, ';')) {
        return false;
    }
    if (!std::getline(lineStream, scoreValue, ';')) {
        return false;
    }
    for (std::size_t i = 0; i < scoreValue.size(); ++i) {
        if (!isdigit(scoreValue[i])) {
            return false;
        }
    }
    scoreEntry.score = std::stoi(scoreValue);
    return true;
}

void ScoreManager::loadScores() {
    _scores.clear();
    std::ifstream scoreFile(_filePath);
    if (!scoreFile.is_open()) {
        return;
    }
    std::string currentLine;
    while (std::getline(scoreFile, currentLine)) {
        if (currentLine.empty()) {
            continue;
        }
        Arcade::ScoreEntry scoreEntry;
        bool isValid = parseScoreLine(currentLine, scoreEntry);
        if (isValid) {
            _scores.push_back(scoreEntry);
        }
    }
    sortScores();
}

void ScoreManager::saveScores() const {
    std::ofstream scoreFile(_filePath);
    if (!scoreFile.is_open()) {
        return;
    }
    for (std::size_t index = 0; index < _scores.size(); ++index) {
        scoreFile << scoreToLine(_scores[index]) << "\n";
    }
}

void ScoreManager::addScore(const Arcade::ScoreEntry &newScore) {
    _scores.push_back(newScore);
    sortScores();
    saveScores();
}

std::vector<Arcade::ScoreEntry> ScoreManager::getAllScores() const {
    return _scores;
}

std::vector<Arcade::ScoreEntry> ScoreManager::getScoresForGame(const std::string &gameName) const {
    std::vector<Arcade::ScoreEntry> gameScores;

    for (std::size_t index = 0; index < _scores.size(); ++index) {
        if (_scores[index].gameName == gameName) {
            gameScores.push_back(_scores[index]);
        }
    }
    return gameScores;
}

std::vector<Arcade::ScoreEntry> ScoreManager::getTopScoresForGame(const std::string &gameName, std::size_t maxScores) const {
    std::vector<Arcade::ScoreEntry> gameScores = getScoresForGame(gameName);
    if (gameScores.size() > maxScores) {
        gameScores.resize(maxScores);
    }
    return gameScores;
}

std::string ScoreManager::scoreToLine(const Arcade::ScoreEntry &scoreEntry) const {
    return scoreEntry.playerName + ";" + scoreEntry.gameName + ";" + std::to_string(scoreEntry.score);
}

bool compareScores(const Arcade::ScoreEntry &leftScore, const Arcade::ScoreEntry &rightScore) {
    if (leftScore.gameName != rightScore.gameName) {
        return leftScore.gameName < rightScore.gameName;
    }
    return leftScore.score > rightScore.score;
}

void ScoreManager::sortScores() {
    std::sort(_scores.begin(), _scores.end(), compareScores);
}