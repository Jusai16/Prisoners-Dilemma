#include "strategies/advanced/AdaptiveStrategy.h"
#include "utils/ConfigFileParser.h"
#include <chrono>
#include <algorithm>
#include <iostream>

AdaptiveStrategy::AdaptiveStrategy()
    : name("AdaptiveStrategy"),
      cooperationLevel(0.7),
      learningRate(0.1),
      explorationRate(0.05),
      memorySize(10),
      totalCooperate(0),
      totalDefect(0),
      averagePayoff(0.0),
      rng(std::chrono::system_clock::now().time_since_epoch().count()),
      dist(0.0, 1.0) {}

Move AdaptiveStrategy::makeMove(
    const std::vector<Move>& ownHistory,
    const std::vector<std::vector<Move>>& opponentsHistory) {
    
    if (ownHistory.empty()) {
        return getRandomMove(cooperationLevel);
    }
    
    updateStatistics(ownHistory.back());
    analyzeOpponentsBehavior(opponentsHistory);
    
    Move decision = decideMove(ownHistory, opponentsHistory);
    
    if (shouldExplore()) {
        return getRandomMove(0.5);
    }
    
    return decision;
}

void AdaptiveStrategy::loadConfig(const std::string& configDir) {
    if (configDir.empty()) return;
    
    ConfigFileParser config;
    if (config.loadFromDir(configDir, "adaptive")) {
        name = config.getString("name", "AdaptiveStrategy");
        cooperationLevel = config.getDouble("initial_cooperation", 0.7);
        learningRate = config.getDouble("learning_rate", 0.1);
        explorationRate = config.getDouble("exploration_rate", 0.05);
        memorySize = config.getInt("memory_size", 10);
        
        // Ограничиваем значения
        cooperationLevel = std::max(0.0, std::min(1.0, cooperationLevel));
        learningRate = std::max(0.0, std::min(1.0, learningRate));
        explorationRate = std::max(0.0, std::min(1.0, explorationRate));
        memorySize = std::max(1, memorySize);
        
        std::cout << "AdaptiveStrategy: Loaded configuration '" << name << "'" << std::endl;
        std::cout << "  Initial cooperation: " << cooperationLevel << std::endl;
        std::cout << "  Learning rate: " << learningRate << std::endl;
        std::cout << "  Exploration rate: " << explorationRate << std::endl;
        std::cout << "  Memory size: " << memorySize << std::endl;
    }
}


Move AdaptiveStrategy::getRandomMove(double cooperateProbability) {
    return (dist(rng) < cooperateProbability) ? Move::COOPERATE : Move::DEFECT;
}

void AdaptiveStrategy::updateStatistics(Move lastMove) {
    if (lastMove == Move::COOPERATE) {
        totalCooperate++;
    } else {
        totalDefect++;
    }
}

void AdaptiveStrategy::analyzeOpponentsBehavior(const std::vector<std::vector<Move>>& opponentsHistory) {
    if (opponentsHistory.empty()) return;
    
    double opponentCooperationRate = calculateOpponentCooperationRate(opponentsHistory);
    cooperationLevel = cooperationLevel * (1.0 - learningRate) + 
                      opponentCooperationRate * learningRate;
    
    cooperationLevel = std::max(0.1, std::min(0.9, cooperationLevel));
}

double AdaptiveStrategy::calculateOpponentCooperationRate(const std::vector<std::vector<Move>>& opponentsHistory) {
    int totalMoves = 0;
    int cooperateMoves = 0;
    
    for (const auto& history : opponentsHistory) {
        if (history.size() > static_cast<size_t>(memorySize)) {
            for (size_t i = history.size() - memorySize; i < history.size(); ++i) {
                totalMoves++;
                if (history[i] == Move::COOPERATE) {
                    cooperateMoves++;
                }
            }
        } else {
            for (const auto& move : history) {
                totalMoves++;
                if (move == Move::COOPERATE) {
                    cooperateMoves++;
                }
            }
        }
    }
    
    return (totalMoves > 0) ? static_cast<double>(cooperateMoves) / totalMoves : 0.5;
}

Move AdaptiveStrategy::decideMove(const std::vector<Move>& ownHistory,
                                  const std::vector<std::vector<Move>>& opponentsHistory) {
    
    double defectRate = calculateOpponentDefectRate(opponentsHistory);
    if (defectRate > 0.7) {
        return Move::DEFECT;
    }
    
    if (defectRate < 0.3) {
        return Move::COOPERATE;
    }
    
    return getRandomMove(cooperationLevel);
}

double AdaptiveStrategy::calculateOpponentDefectRate(const std::vector<std::vector<Move>>& opponentsHistory) {
    int totalMoves = 0;
    int defectMoves = 0;
    
    for (const auto& history : opponentsHistory) {
        for (const auto& move : history) {
            totalMoves++;
            if (move == Move::DEFECT) {
                defectMoves++;
            }
        }
    }
    
    return (totalMoves > 0) ? static_cast<double>(defectMoves) / totalMoves : 0.0;
}

bool AdaptiveStrategy::shouldExplore() {
    return dist(rng) < explorationRate;
}
