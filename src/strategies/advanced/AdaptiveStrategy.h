#ifndef ADAPTIVESTRATEGY_H
#define ADAPTIVESTRATEGY_H

#include "core/Strategy.h"
#include "utils/ConfigFileParser.h"
#include <string>
#include <vector>
#include <random>

class AdaptiveStrategy : public Strategy {
private:
    std::string name;
    double cooperationLevel;
    double learningRate;
    double explorationRate;
    int memorySize;
    std::vector<double> recentPayoffs;
    
    int totalCooperate;
    int totalDefect;
    double averagePayoff;
    
    std::mt19937 rng;
    std::uniform_real_distribution<double> dist;
    
    Move getRandomMove(double cooperateProbability);
    void updateStatistics(Move lastMove);
    void analyzeOpponentsBehavior(const std::vector<std::vector<Move>>& opponentsHistory);
    double calculateOpponentCooperationRate(const std::vector<std::vector<Move>>& opponentsHistory);
    Move decideMove(const std::vector<Move>& ownHistory,
                    const std::vector<std::vector<Move>>& opponentsHistory);
    double calculateOpponentDefectRate(const std::vector<std::vector<Move>>& opponentsHistory);
    bool shouldExplore();
    
public:
    AdaptiveStrategy();
    ~AdaptiveStrategy() override = default;
    
    Move makeMove(const std::vector<Move>& ownHistory,
                  const std::vector<std::vector<Move>>& opponentsHistory) override;
    
    std::string getName() const override { return name; }
    
    void loadConfig(const std::string& configDir) override;
    
    double getCooperationLevel() const { return cooperationLevel; }
    int getTotalCooperate() const { return totalCooperate; }
    int getTotalDefect() const { return totalDefect; }
    double getAveragePayoff() const { return averagePayoff; }
};

#endif