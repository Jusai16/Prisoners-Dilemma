#ifndef ALWAYSDEFECT_H
#define ALWAYSDEFECT_H

#include "core/Strategy.h"

class AlwaysDefect : public Strategy {
public:
    Move makeMove(const std::vector<Move>& ownHistory, const std::vector<std::vector<Move>>& opponentsHistory) override;
    std::string getName() const override;
};

#endif