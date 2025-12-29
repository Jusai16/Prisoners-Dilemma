#include "strategies/basic/AlwaysCooperate.h"
#include <string>

Move AlwaysCooperate::makeMove(const std::vector<Move>& ownHistory, const std::vector<std::vector<Move>>& opponentsHistory) {
    return Move::COOPERATE;
}

std::string AlwaysCooperate::getName() const {
    return "AlwaysCooperate";
}