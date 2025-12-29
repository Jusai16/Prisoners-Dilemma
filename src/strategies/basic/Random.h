#ifndef RANDOMSTRATEGY_H
#define RANDOMSTRATEGY_H

#include "core/Strategy.h"
#include <vector>
#include <string>

class RandomStrategy : public Strategy {
public:
    RandomStrategy();
    
    Move makeMove(const std::vector<Move>& ownHistory,
                  const std::vector<std::vector<Move>>& opponentsHistory) override;
    
    std::string getName() const override;
};

#endif