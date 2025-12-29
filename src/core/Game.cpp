#include "Game.h"
#include <iostream>
#include <iomanip>

Game::Game(int rounds, const std::string& matrixFile) : currentRound(0), totalRounds(rounds), matrix(matrixFile) {
    scores.resize(3, 0);
    history.resize(3);
    currentMoves.resize(3);
}

void Game::addPlayer(std::unique_ptr<Strategy> player) {
    players.push_back(std::move(player));
}

void Game::playRound() {
    if (!isReady()) {
        std::cerr << "Game is not ready! Need exactly 3 players." << std::endl;
        return;
    }

    for (int i = 0; i < 3; ++i) {
        std::vector<std::vector<Move>> opponentsHistory;
        for (int j = 0; j < 3; ++j) {
            if (i != j) {
                opponentsHistory.push_back(history[j]);
            }
        }

        currentMoves[i] = players[i]->makeMove(history[i], opponentsHistory);
    }

    std::vector<int> roundScores = matrix.getPayoff(currentMoves[0], currentMoves[1], currentMoves[2]);

    for (int i = 0; i < 3; ++i) {
        scores[i] += roundScores[i];
    }

    for (int i = 0; i < 3; ++i) {
        history[i].push_back(currentMoves[i]);
    }

    currentRound++;
}

void Game::playGame() {
    while (currentRound < totalRounds) {
        playRound();
    }
}

std::vector<int> Game::getScores() const {
    return scores;
}

std::vector<std::string> Game::getPlayerNames() const {
    std::vector<std::string> names;
    for (const auto& player : players) {
        names.push_back(player->getName());
    }
    return names;
}

int Game::getCurrentRound() const {
    return currentRound;
}

bool Game::isReady() const {
    return players.size() == 3;
}

void Game::reset() {
    players.clear();
    history.clear();
    currentMoves.clear();
    scores.clear();
    currentRound = 0;
    
    // Пересоздаем векторы
    scores.resize(3, 0);
    history.resize(3);
    currentMoves.resize(3);
}

void Game::printMatrix() const {
    std::cout << "\n=== GAME MATRIX ===" << std::endl;
    matrix.printMatrix();
    std::cout << "===================\n" << std::endl;
}

void Game::printRoundInfo() const {
    if (currentRound == 0) {
        std::cout << "Game hasn't started yet." << std::endl;
        return;
    }
    
    std::cout << "\n=== Round " << currentRound << " ===" << std::endl;
    
    std::cout << "Moves: ";
    for (size_t i = 0; i < players.size(); ++i) {
        std::cout << players[i]->getName() << ": " 
                  << moveToChar(currentMoves[i]);
        if (i < players.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    
    std::vector<int> roundScores = matrix.getPayoff(
        currentMoves[0], currentMoves[1], currentMoves[2]);
    
    std::cout << "Round scores: ";
    for (size_t i = 0; i < players.size(); ++i) {
        std::cout << players[i]->getName() << ": " << roundScores[i];
        if (i < players.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    
    std::cout << "Total scores: ";
    for (size_t i = 0; i < players.size(); ++i) {
        std::cout << players[i]->getName() << ": " << scores[i];
        if (i < players.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    std::cout << "===================\n" << std::endl;
}

void Game::printFinalResults() const {
    std::cout << "\n=========================================" << std::endl;
    std::cout << "FINAL RESULTS" << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "Total rounds played: " << currentRound << std::endl;
    
    int maxScore = -1;
    int winnerIndex = -1;
    
    for (size_t i = 0; i < players.size(); ++i) {
        std::cout << players[i]->getName() << ": " << scores[i] << " points" << std::endl;
        
        if (scores[i] > maxScore) {
            maxScore = scores[i];
            winnerIndex = i;
        }
    }
    
    std::cout << std::string(40, '-') << std::endl;
    
    if (winnerIndex != -1) {
        std::cout << "WINNER: " << players[winnerIndex]->getName() 
                  << " with " << maxScore << " points!" << std::endl;
    }
    
    std::cout << "=========================================\n" << std::endl;
}
