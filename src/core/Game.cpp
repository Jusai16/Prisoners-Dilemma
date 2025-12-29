#include "Game.h"
#include <iostream>
#include <iomanip>

Game::Game(int rounds, const std::string& matrixFile) 
    : currentRound(0), totalRounds(rounds), matrix(matrixFile) {
}

void Game::addPlayer(std::unique_ptr<Strategy> player) {
    players.addPlayer(std::move(player));
}

void Game::playRound() {
    if (!isReady()) {
        std::cerr << "Game is not ready! Need exactly 3 players." << std::endl;
        return;
    }

    // 1. Каждый игрок делает ход
    for (int i = 0; i < 3; ++i) {
        auto opponentsHistory = players.getOpponentsHistory(i);
        auto ownHistory = players.getPlayerHistory(i);
        
        Move move = players.getStrategies()[i]->makeMove(ownHistory, opponentsHistory);
        players.setCurrentMove(i, move);
    }

    // 2. Получаем очки за раунд
    auto currentMoves = players.getCurrentMoves();
    std::vector<int> roundScores = matrix.getPayoff(
        currentMoves[0], currentMoves[1], currentMoves[2]);

    // 3. Обновляем очки
    for (int i = 0; i < 3; ++i) {
        players.addToScore(i, roundScores[i]);
    }

    // 4. Сохраняем ходы в историю
    for (int i = 0; i < 3; ++i) {
        players.addMoveToHistory(i, currentMoves[i]);
    }

    currentRound++;
}

void Game::playGame() {
    while (currentRound < totalRounds) {
        playRound();
    }
}

std::vector<int> Game::getScores() const {
    return players.getScores();
}

std::vector<std::string> Game::getPlayerNames() const {
    return players.getNames();
}

int Game::getCurrentRound() const {
    return currentRound;
}

bool Game::isReady() const {
    return players.hasThreePlayers();
}

void Game::reset() {
    players.clear();
    players.resizeForThreePlayers();
    currentRound = 0;
}

std::vector<Move> Game::getCurrentMoves() const {
    return players.getCurrentMoves();
}

void Game::printMatrix() const {
    std::cout << "\n=== GAME MATRIX ===" << std::endl;
    matrix.printMatrix();
}

void Game::printRoundInfo() const {
    if (currentRound == 0) {
        std::cout << "Game hasn't started yet." << std::endl;
        return;
    }
    
    auto names = players.getNames();
    auto currentMoves = players.getCurrentMoves();
    auto scores = players.getScores();
    
    std::cout << "\n=== Round " << currentRound << " ===" << std::endl;
    
    std::cout << "Moves: ";
    for (size_t i = 0; i < names.size(); ++i) {
        std::cout << names[i] << ": " << moveToChar(currentMoves[i]);
        if (i < names.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    
    std::vector<int> roundScores = matrix.getPayoff(
        currentMoves[0], currentMoves[1], currentMoves[2]);
    
    std::cout << "Round scores: ";
    for (size_t i = 0; i < names.size(); ++i) {
        std::cout << names[i] << ": " << roundScores[i];
        if (i < names.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    
    std::cout << "Total scores: ";
    for (size_t i = 0; i < names.size(); ++i) {
        std::cout << names[i] << ": " << scores[i];
        if (i < names.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    std::cout << "===================\n" << std::endl;
}

void Game::printFinalResults() const {
    auto names = players.getNames();
    auto scores = players.getScores();
    
    std::cout << "\n=========================================" << std::endl;
    std::cout << "FINAL RESULTS" << std::endl;
    std::cout << "Total rounds played: " << currentRound << std::endl;
    
    int maxScore = -1;
    int winnerIndex = -1;
    
    for (size_t i = 0; i < names.size(); ++i) {
        std::cout << names[i] << ": " << scores[i] << " points" << std::endl;
        
        if (scores[i] > maxScore) {
            maxScore = scores[i];
            winnerIndex = i;
        }
    }
    
    std::cout << std::string(40, '-') << std::endl;
    
    if (winnerIndex != -1) {
        std::cout << "WINNER: " << names[winnerIndex] 
                  << " with " << maxScore << " points!" << std::endl;
    }
    
    std::cout << "=========================================\n" << std::endl;
}