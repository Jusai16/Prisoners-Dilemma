#ifndef GAMEMATRIX_H
#define GAMEMATRIX_H

#include <vector>
#include <string>
#include <fstream>
#include "core/Strategy.h"

class GameMatrix {
private:
    int payoff[2][2][2][3];
    bool matrixLoaded;
    
    void parseLine(const std::string& line);
    int moveToIndex(Move move) const {
        return (move == Move::COOPERATE) ? 0 : 1;
    }
    
public:
    GameMatrix();
    GameMatrix(const std::string& filename);
    
    bool loadFromFile(const std::string& filename);
    std::vector<int> getPayoff(Move move1, Move move2, Move move3) const;
    void setDefaultMatrix();
    void printMatrix() const;
    
    bool isMatrixLoaded() const { return matrixLoaded; }
};

#endif 