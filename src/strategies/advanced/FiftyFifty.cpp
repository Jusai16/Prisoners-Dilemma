#include "strategies/advanced/FiftyFifty.h"
#include "utils/ConfigFileParser.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>

FiftyFifty::FiftyFifty() : name("FiftyFifty"), lastMoveRandom(false), firstMove('C') {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

Move FiftyFifty::makeMove(const std::vector<Move>& ownHistory,
                          const std::vector<std::vector<Move>>& opponentsHistory) {
    
    if (ownHistory.empty()) {
        lastMoveRandom = true;
        return (firstMove == 'C') ? Move::COOPERATE : Move::DEFECT;
    }
    
    lastMoveRandom = false;
    return (ownHistory.back() == Move::COOPERATE) ? Move::DEFECT : Move::COOPERATE;
}

std::string FiftyFifty::getName() const {
    return name;
}

void FiftyFifty::loadConfig(const std::string& configDir) {
    if (configDir.empty()) return;
    
    ConfigFileParser config;
    if (config.loadFromDir(configDir, "fiftyfifty")) {
        name = config.getString("name", "FiftyFifty");
        
        std::string firstMoveStr = config.getString("first_move", "C");
        if (!firstMoveStr.empty()) {
            firstMove = firstMoveStr[0];
        }
        
        std::cout << "FiftyFifty: Loaded configuration '" << name 
                  << "', first move: " << firstMove << std::endl;
    }
}

bool FiftyFifty::isLastMoveRandom() const {
    return lastMoveRandom;
}