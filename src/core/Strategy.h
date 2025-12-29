#ifndef STRATEGY_H
#define STRATEGY_H

#include <vector>
#include <string>
#include <functional>

enum class Move {
    COOPERATE,
    DEFECT
};

inline char moveToChar(Move move) {
    return (move == Move::COOPERATE) ? 'C' : 'D';
}

inline Move charToMove(char c) {
    return (c == 'C' || c == 'c') ? Move::COOPERATE : Move::DEFECT;
}

// Базовый абстрактный класс стратегии
class Strategy {
public:
    virtual ~Strategy() = default;
    
    virtual Move makeMove(
        const std::vector<Move>& ownHistory,
        const std::vector<std::vector<Move>>& opponentsHistory
    ) = 0;
    
    virtual std::string getName() const = 0;
    
    // Виртуальный метод для загрузки конфигурации
    virtual void loadConfig(const std::string& configDir) {
    }
};

#endif