#include "Tournament.h"
#include "StrategyFactory.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

Tournament::Tournament(const std::vector<std::string>& strategies, 
                       int rounds, 
                       const std::string& configDir,
                       const std::string& matrixFile) 
    : strategyNames(strategies), 
      configDir(configDir), 
      matrixFile(matrixFile), 
      roundsPerGame(rounds) {
    
    for (const auto& name : strategyNames) {
        totalScores[name] = 0;
    }
}

void Tournament::run() {
    auto triplets = generateTriplets();
    
    std::cout << "Starting tournament with " << strategyNames.size() 
              << " strategies" << std::endl;
    std::cout << "Number of unique triplets: " << triplets.size() << std::endl;
    std::cout << "Rounds per game: " << roundsPerGame << std::endl;
    
    int gameCount = 1;
    for (const auto& triplet : triplets) {
        std::cout << "\nGame " << gameCount++ << "/" << triplets.size() 
                  << ": " << triplet[0] << " vs " << triplet[1] << " vs " << triplet[2] << std::endl;
        
        playTriplet(triplet);
    }
}

std::vector<std::vector<std::string>> Tournament::generateTriplets() const {
    std::vector<std::vector<std::string>> triplets;
    
    // Генерируем все уникальные комбинации из 3 стратегий
    size_t n = strategyNames.size();
    if (n < 3) return triplets;
    
    std::vector<int> indices(n);
    for (size_t i = 0; i < n; ++i) indices[i] = i;
    
    do {
        std::vector<std::string> triplet = {
            strategyNames[indices[0]],
            strategyNames[indices[1]],
            strategyNames[indices[2]]
        };
        
        // Сортируем для уникальности (порядок не важен)
        std::sort(triplet.begin(), triplet.end());
        
        // Проверяем, есть ли уже такая тройка
        bool exists = false;
        for (const auto& existing : triplets) {
            if (existing[0] == triplet[0] && 
                existing[1] == triplet[1] && 
                existing[2] == triplet[2]) {
                exists = true;
                break;
            }
        }
        
        if (!exists) {
            triplets.push_back(triplet);
        }
        
        // Пропускаем оставшиеся комбинации для этих индексов
        std::reverse(indices.begin() + 3, indices.end());
        
    } while (std::next_permutation(indices.begin(), indices.end()));
    
    return triplets;
}

void Tournament::playTriplet(const std::vector<std::string>& triplet) {
    Game game(roundsPerGame, matrixFile);
    
    auto& factory = StrategyFactory::getInstance();
    for (const auto& name : triplet) {
        auto strategy = factory.create(name, configDir);
        if (strategy) {
            game.addPlayer(std::move(strategy));
        }
    }
    
    if (game.isReady()) {
        game.playGame();
        auto scores = game.getScores();
        auto names = game.getPlayerNames();
        
        for (size_t i = 0; i < names.size(); ++i) {
            totalScores[names[i]] += scores[i];
        }
        
        std::cout << "Game results: ";
        for (size_t i = 0; i < names.size(); ++i) {
            std::cout << names[i] << "=" << scores[i];
            if (i < names.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
}

void Tournament::printResults() const {
    std::cout << "\n=========================================" << std::endl;
    std::cout << "TOURNAMENT FINAL RESULTS" << std::endl;
    std::cout << "=========================================" << std::endl;
    
    std::vector<std::pair<std::string, int>> sortedScores(
        totalScores.begin(), totalScores.end());
    
    std::sort(sortedScores.begin(), sortedScores.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });
    
    std::cout << std::left << std::setw(25) << "Strategy" 
              << std::right << std::setw(10) << "Score" << std::endl;
    std::cout << std::string(35, '-') << std::endl;
    
    for (const auto& [name, score] : sortedScores) {
        std::cout << std::left << std::setw(25) << name 
                  << std::right << std::setw(10) << score << std::endl;
    }
    
    if (!sortedScores.empty()) {
        std::cout << "\nTOURNAMENT WINNER: " << sortedScores[0].first 
                  << " with " << sortedScores[0].second << " points!" << std::endl;
    }
}

std::string Tournament::getWinner() const {
    auto it = std::max_element(totalScores.begin(), totalScores.end(),
                               [](const auto& a, const auto& b) {
                                   return a.second < b.second;
                               });
    return it != totalScores.end() ? it->first : "";
}