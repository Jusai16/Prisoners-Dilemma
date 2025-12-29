#ifndef TITFORTAT_H
#define TITFORTAT_H

#include "core/Strategy.h"
#include "utils/ConfigFileParser.h"
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>

class TitForTat : public Strategy {
private:
    std::string name;
    Move firstMove;
    bool useForgiveness;
    double forgivenessProbability;
    std::mt19937 rng;
    std::uniform_real_distribution<double> dist;
    
    Move analyzeOpponentsLastMoves(const std::vector<std::vector<Move>>& opponentsHistory);
    bool shouldForgive();
    
public:
    TitForTat();
    ~TitForTat() override = default;
    
    Move makeMove(const std::vector<Move>& ownHistory,
                  const std::vector<std::vector<Move>>& opponentsHistory) override;
    
    std::string getName() const override { return name; }
    
    void loadConfig(const std::string& configDir) override;
};

#endif