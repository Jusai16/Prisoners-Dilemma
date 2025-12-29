#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include <string>
#include "core/Strategy.h"
#include "core/GameMatrix.h"

class Game {
private:
    std::vector<std::unique_ptr<Strategy>> players;
    std::vector<std::vector<Move>> history;
    std::vector<Move> currentMoves;
    std::vector<int> scores;
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
    
    std::vector<Move> getCurrentMoves() const { return currentMoves; }
};

#endif