#ifndef ALWAYSCOOPERATE_H
#define ALWAYSCOOPERATE_H

#include "core/Strategy.h"

class AlwaysCooperate : public Strategy {
public:
    Move makeMove(const std::vector<Move>& ownHistory, const std::vector<std::vector<Move>>& opponentsHistory) override;
    std::string getName() const override;
};

#endif