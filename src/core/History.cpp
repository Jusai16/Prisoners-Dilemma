#include "History.h"
#include <fstream>
#include <iostream>
#include <sstream>

History::History(const std::string& configDir) : configPath(configDir) {
    if (!configDir.empty()) {
        loadFromFile();
    }
}

void History::addRound(const std::vector<std::string>& playerNames, 
                       const std::vector<Move>& moves) {
    if (playerNames.size() != moves.size()) {
        throw std::invalid_argument("Player names and moves size mismatch");
    }
    
    RoundRecord record;
    for (size_t i = 0; i < playerNames.size(); ++i) {
        record[playerNames[i]] = moves[i];
    }
    rounds.push_back(record);
}

RoundRecord History::getLastRound() const {
    if (rounds.empty()) {
        return RoundRecord();
    }
    return rounds.back();
}

std::vector<Move> History::getPlayerMoves(const std::string& playerName) const {
    std::vector<Move> result;
    for (const auto& round : rounds) {
        auto it = round.find(playerName);
        if (it != round.end()) {
            result.push_back(it->second);
        }
    }
    return result;
}

std::vector<RoundRecord> History::getAllRounds() const {
    return rounds;
}

bool History::isEmpty() const {
    return rounds.empty();
}

void History::clear() {
    rounds.clear();
}

void History::saveToFile() const {
    if (configPath.empty()) return;
    
    std::ofstream file(configPath + "/history.txt", std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Warning: Cannot save history to " << configPath << "/history.txt" << std::endl;
        return;
    }
    
    file.exceptions(std::ifstream::badbit);
    
    for (const auto& round : rounds) {
        for (const auto& [name, move] : round) {
            file << name << ":" << moveToChar(move) << " ";
        }
        file << "\n";
    }
}

bool History::loadFromFile() {
    if (configPath.empty()) return false;
    
    std::ifstream file(configPath + "/history.txt");
    if (!file.is_open()) {
        return false;
    }
    
    file.exceptions(std::ifstream::badbit);
    
    try {
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            
            std::istringstream iss(line);
            std::string token;
            RoundRecord record;
            
            while (iss >> token) {
                size_t colonPos = token.find(':');
                if (colonPos == std::string::npos) continue;
                
                std::string name = token.substr(0, colonPos);
                char moveChar = token[colonPos + 1];
                Move move = (moveChar == 'C' || moveChar == 'c') ? Move::COOPERATE : Move::DEFECT;
                
                record[name] = move;
            }
            
            if (!record.empty()) {
                rounds.push_back(record);
            }
        }
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading history: " << e.what() << std::endl;
        return false;
    }
}