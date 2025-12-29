#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include <vector>
#include <string>
#include <memory>
#include <map>
#include "core/Strategy.h"
#include "core/Game.h"

class Tournament {
private:
    std::vector<std::string> strategyNames;
    std::string configDir;
    std::string matrixFile;
    int roundsPerGame;
    std::map<std::string, int> totalScores;
    
public:
    Tournament(const std::vector<std::string>& strategies, 
               int rounds = 100, 
               const std::string& configDir = "",
               const std::string& matrixFile = "");
    
    void run();
    void printResults() const;
    std::string getWinner() const;
    std::map<std::string, int> getScores() const { return totalScores; }
    
private:
    void playTriplet(const std::vector<std::string>& triplet);
    std::vector<std::vector<std::string>> generateTriplets() const;
};

#endif