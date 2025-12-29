#include "strategies/advanced/TitForTat.h"
#include <chrono>

TitForTat::TitForTat() 
    : name("TitForTat"),
      firstMove(Move::COOPERATE),
      useForgiveness(true),
      forgivenessProbability(0.1),
      rng(std::chrono::system_clock::now().time_since_epoch().count()),
      dist(0.0, 1.0) {}

Move TitForTat::makeMove(const std::vector<Move>& ownHistory,
                        const std::vector<std::vector<Move>>& opponentsHistory) {
    if (ownHistory.empty()) {
        return firstMove;
    }
    
    Move response = analyzeOpponentsLastMoves(opponentsHistory);
    
    if (useForgiveness && shouldForgive()) {
        return Move::COOPERATE;
    }
    
    return response;
}

void TitForTat::loadConfig(const std::string& configDir) {
    if (configDir.empty()) return;
    
    ConfigFileParser config;
    if (config.loadFromDir(configDir, "titfortat")) {
        name = config.getString("name", "TitForTat");
        
        std::string firstMoveStr = config.getString("first_move", "C");
        firstMove = (firstMoveStr == "C" || firstMoveStr == "c") ? 
                    Move::COOPERATE : Move::DEFECT;
        
        useForgiveness = config.getBool("use_forgiveness", true);
        forgivenessProbability = config.getDouble("forgiveness_probability", 0.1);
        
        // Ограничиваем вероятность
        if (forgivenessProbability < 0.0) forgivenessProbability = 0.0;
        if (forgivenessProbability > 1.0) forgivenessProbability = 1.0;
        
        std::cout << "TitForTat: Loaded configuration '" << name << "'" << std::endl;
        std::cout << "  First move: " << moveToChar(firstMove) << std::endl;
        std::cout << "  Forgiveness: " << (useForgiveness ? "enabled" : "disabled") << std::endl;
        std::cout << "  Forgiveness probability: " << forgivenessProbability << std::endl;
    }
}

Move TitForTat::analyzeOpponentsLastMoves(const std::vector<std::vector<Move>>& opponentsHistory) {
    if (opponentsHistory.empty()) {
        return Move::COOPERATE;
    }
    
    int defectCount = 0;
    int totalOpponents = 0;
    
    for (const auto& history : opponentsHistory) {
        if (!history.empty()) {
            totalOpponents++;
            if (history.back() == Move::DEFECT) {
                defectCount++;
            }
        }
    }
    
    if (totalOpponents == 0) {
        return Move::COOPERATE;
    }
    
    return (defectCount > totalOpponents / 2) ? Move::DEFECT : Move::COOPERATE;
}

bool TitForTat::shouldForgive() {
    return dist(rng) < forgivenessProbability;
}