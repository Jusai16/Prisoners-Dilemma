#ifndef PLAYERS_H
#define PLAYERS_H

#include <vector>
#include <memory>
#include <string>
#include "core/Strategy.h"

class Players {
private:
    std::vector<std::unique_ptr<Strategy>> strategies;
    std::vector<std::vector<Move>> history;
    std::vector<int> scores;
    std::vector<Move> currentMoves;
    
public:
    Players();
    
    // Управление игроками
    void addPlayer(std::unique_ptr<Strategy> player);
    void clear();
    void resizeForThreePlayers();
    bool hasThreePlayers() const;
    size_t count() const { return strategies.size(); }
    
    // Доступ к данным
    const std::vector<std::unique_ptr<Strategy>>& getStrategies() const { return strategies; }
    std::vector<std::string> getNames() const;
    std::vector<int> getScores() const { return scores; }
    int getScore(size_t index) const { return scores[index]; }
    
    // Работа с историей
    void addMoveToHistory(size_t playerIndex, Move move);
    const std::vector<Move>& getPlayerHistory(size_t playerIndex) const { return history[playerIndex]; }
    std::vector<std::vector<Move>> getOpponentsHistory(size_t playerIndex) const;
    
    // Текущие ходы
    void setCurrentMove(size_t playerIndex, Move move);
    Move getCurrentMove(size_t playerIndex) const { return currentMoves[playerIndex]; }
    const std::vector<Move>& getCurrentMoves() const { return currentMoves; }
    
    // Обновление очков
    void addToScore(size_t playerIndex, int points);
    
    // Сброс состояния
    void resetForNewGame();
};

#endif