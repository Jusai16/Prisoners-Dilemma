#include "strategies/basic/AlwaysDefect.h"
#include <string>

Move AlwaysDefect::makeMove(const std::vector<Move>& ownHistory, const std::vector<std::vector<Move>>& opponentsHistory) {
    return Move::DEFECT;
}

std::string AlwaysDefect::getName() const {
    return "AlwaysDefect";
}