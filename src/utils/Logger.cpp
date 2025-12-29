#include "utils/Logger.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <map>
#include <algorithm>

Logger::Logger(const std::string& configDir) : enabled(true) {
    if (!configDir.empty()) {
        logPath = configDir + "/game_log.txt";
        logFile.open(logPath, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Warning: Cannot open log file at " << logPath << std::endl;
            enabled = false;
        }
    } else {
        enabled = false;
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::logRound(int round, 
                      const std::vector<std::string>& playerNames,
                      const std::vector<Move>& moves,
                      const std::vector<int>& roundScores,
                      const std::vector<int>& totalScores) {
    if (!enabled) return;
    
    logFile << getTimestamp() << " | Round " << round << " | ";
    
    for (size_t i = 0; i < playerNames.size(); ++i) {
        logFile << playerNames[i] << "=" << moveToString(moves[i]) 
                << "(" << roundScores[i] << ") ";
    }
    
    logFile << "| Total: ";
    for (size_t i = 0; i < playerNames.size(); ++i) {
        logFile << playerNames[i] << "=" << totalScores[i];
        if (i < playerNames.size() - 1) logFile << ", ";
    }
    
    logFile << std::endl;
}

void Logger::logGameStart(const std::vector<std::string>& playerNames, int totalRounds) {
    if (!enabled) return;
    
    logFile << "\n" << std::string(60, '=') << std::endl;
    logFile << getTimestamp() << " | GAME STARTED" << std::endl;
    logFile << "Players: ";
    for (const auto& name : playerNames) {
        logFile << name << " ";
    }
    logFile << "| Rounds: " << totalRounds << std::endl;
    logFile << std::string(60, '=') << std::endl;
}

void Logger::logGameEnd(const std::vector<std::string>& playerNames,
                        const std::vector<int>& finalScores) {
    if (!enabled) return;
    
    logFile << std::string(60, '=') << std::endl;
    logFile << getTimestamp() << " | GAME ENDED" << std::endl;
    
    int maxScore = -1;
    int winnerIndex = -1;
    
    for (size_t i = 0; i < playerNames.size(); ++i) {
        logFile << playerNames[i] << ": " << finalScores[i] << " points" << std::endl;
        
        if (finalScores[i] > maxScore) {
            maxScore = finalScores[i];
            winnerIndex = i;
        }
    }
    
    if (winnerIndex != -1) {
        logFile << "WINNER: " << playerNames[winnerIndex] 
                << " with " << maxScore << " points!" << std::endl;
    }
    
    logFile << std::string(60, '=') << std::endl;
}

void Logger::logTournamentStart(const std::vector<std::string>& allStrategies) {
    if (!enabled) return;
    
    logFile << "\n" << std::string(70, '=') << std::endl;
    logFile << getTimestamp() << " | TOURNAMENT STARTED" << std::endl;
    logFile << "Participating strategies (" << allStrategies.size() << "): ";
    for (const auto& name : allStrategies) {
        logFile << name << " ";
    }
    logFile << std::endl;
    logFile << std::string(70, '=') << std::endl;
}

void Logger::logTournamentEnd(const std::map<std::string, int>& finalScores) {
    if (!enabled) return;
    
    logFile << std::string(70, '=') << std::endl;
    logFile << getTimestamp() << " | TOURNAMENT ENDED" << std::endl;
    
    std::vector<std::pair<std::string, int>> sorted(
        finalScores.begin(), finalScores.end());
    
    std::sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });
    
    for (const auto& [name, score] : sorted) {
        logFile << name << ": " << score << " points" << std::endl;
    }
    
    if (!sorted.empty()) {
        logFile << "TOURNAMENT WINNER: " << sorted[0].first 
                << " with " << sorted[0].second << " points!" << std::endl;
    }
    
    logFile << std::string(70, '=') << std::endl;
}

std::string Logger::moveToString(Move move) const {
    return (move == Move::COOPERATE) ? "C" : "D";
}

std::string Logger::getTimestamp() const {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
    
    return std::string(buffer);
}