#include "strategies/basic/Random.h"
#include <cstdlib>
#include <ctime>
#include <string>

RandomStrategy::RandomStrategy() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

Move RandomStrategy::makeMove(const std::vector<Move>& ownHistory,
                              const std::vector<std::vector<Move>>& opponentsHistory) {
    return (std::rand() % 2 == 0) ? Move::COOPERATE : Move::DEFECT;
}

std::string RandomStrategy::getName() const {
    return "RandomStrategy";
}