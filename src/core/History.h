#ifndef HISTORY_H
#define HISTORY_H

#include <vector>
#include <map>
#include <string>
#include "core/Strategy.h"

using RoundRecord = std::map<std::string, Move>;

class History {
private:
    std::vector<RoundRecord> rounds;
    std::string configPath;
    
public:
    History(const std::string& configDir = "");
    
    void addRound(const std::vector<std::string>& playerNames, 
                  const std::vector<Move>& moves);
    RoundRecord getLastRound() const;
    std::vector<Move> getPlayerMoves(const std::string& playerName) const;
    std::vector<RoundRecord> getAllRounds() const;
    bool isEmpty() const;
    void clear();
    void saveToFile() const;
    bool loadFromFile();
    
    int getRoundCount() const { return rounds.size(); }
};

#endif