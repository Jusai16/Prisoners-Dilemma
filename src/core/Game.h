#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include <string>
#include "core/Strategy.h"
#include "core/GameMatrix.h"
#include "core/Players.h"

class Game {
private:
    Players players;
    GameMatrix matrix;
    int currentRound;
    int totalRounds;
    
public:
    Game(int rounds = 100, const std::string& matrixFile = "");
    void addPlayer(std::unique_ptr<Strategy> player);
    void playRound();
    void playGame();

    std::vector<int> getScores() const;
    std::vector<std::string> getPlayerNames() const;
    int getCurrentRound() const;
    int getTotalRounds() const { return totalRounds; }

    void printRoundInfo() const;
    void printFinalResults() const;
    void printMatrix() const;

    bool isReady() const;
    void reset();
    
    std::vector<Move> getCurrentMoves() const;
};

#endif