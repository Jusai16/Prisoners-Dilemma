#include "Players.h"
#include <algorithm>

Players::Players() {
    resizeForThreePlayers();
}

void Players::addPlayer(std::unique_ptr<Strategy> player) {
    strategies.push_back(std::move(player));
}

void Players::clear() {
    strategies.clear();
    history.clear();
    scores.clear();
    currentMoves.clear();
}

void Players::resizeForThreePlayers() {
    history.resize(3);
    scores.resize(3, 0);
    currentMoves.resize(3);
}

bool Players::hasThreePlayers() const {
    return strategies.size() == 3;
}

std::vector<std::string> Players::getNames() const {
    std::vector<std::string> names;
    for (const auto& strategy : strategies) {
        names.push_back(strategy->getName());
    }
    return names;
}

void Players::addMoveToHistory(size_t playerIndex, Move move) {
    if (playerIndex < history.size()) {
        history[playerIndex].push_back(move);
    }
}

std::vector<std::vector<Move>> Players::getOpponentsHistory(size_t playerIndex) const {
    std::vector<std::vector<Move>> opponentsHistory;
    for (size_t i = 0; i < history.size(); ++i) {
        if (i != playerIndex) {
            opponentsHistory.push_back(history[i]);
        }
    }
    return opponentsHistory;
}

void Players::setCurrentMove(size_t playerIndex, Move move) {
    if (playerIndex < currentMoves.size()) {
        currentMoves[playerIndex] = move;
    }
}

void Players::addToScore(size_t playerIndex, int points) {
    if (playerIndex < scores.size()) {
        scores[playerIndex] += points;
    }
}

void Players::resetForNewGame() {
    // Очищаем историю и сбрасываем счет, но оставляем стратегии
    for (auto& h : history) h.clear();
    std::fill(scores.begin(), scores.end(), 0);
    std::fill(currentMoves.begin(), currentMoves.end(), Move::COOPERATE);
}