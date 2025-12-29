#include "core/GameMatrix.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

GameMatrix::GameMatrix() : matrixLoaded(false) {
    setDefaultMatrix();
}

GameMatrix::GameMatrix(const std::string& filename) : matrixLoaded(false) {
    if (!filename.empty()) {
        if (loadFromFile(filename)) {
            matrixLoaded = true;
        } else {
            std::cerr << "Warning: Failed to load matrix from " << filename
                      << ". Using default matrix." << std::endl;
            setDefaultMatrix();
        }
    } else {
        setDefaultMatrix();
    }
}

bool GameMatrix::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open matrix file " << filename << std::endl;
        return false;
    }

    file.exceptions(std::ifstream::badbit);

    try {
        std::string line;
        int lineNumber = 0;
        
        while (std::getline(file, line)) {
            lineNumber++;
            
            // Пропускаем пустые строки и комментарии
            if (line.empty() || line[0] == '#') {
                continue;
            }
            
            // Удаляем начальные и конечные пробелы
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);
            
            if (line.empty()) continue;
            
            parseLine(line);
        }
        
        std::cout << "Successfully loaded matrix from " << filename 
                  << " (" << lineNumber << " lines processed)" << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error reading matrix file: " << e.what() << std::endl;
        return false;
    }
}

void GameMatrix::parseLine(const std::string& line) {
    std::istringstream iss(line);
    char m1, m2, m3;
    int p1, p2, p3;

    if (iss >> m1 >> m2 >> m3 >> p1 >> p2 >> p3) {
        int idx1 = (m1 == 'C' || m1 == 'c') ? 0 : 1;
        int idx2 = (m2 == 'C' || m2 == 'c') ? 0 : 1;
        int idx3 = (m3 == 'C' || m3 == 'c') ? 0 : 1;

        payoff[idx1][idx2][idx3][0] = p1;
        payoff[idx1][idx2][idx3][1] = p2;
        payoff[idx1][idx2][idx3][2] = p3;
        
    } else {
        std::cerr << "Warning: Cannot parse matrix line: " << line << std::endl;
    }
}

std::vector<int> GameMatrix::getPayoff(Move move1, Move move2, Move move3) const {
    int idx1 = moveToIndex(move1);
    int idx2 = moveToIndex(move2);
    int idx3 = moveToIndex(move3);
    
    return {
        payoff[idx1][idx2][idx3][0],
        payoff[idx1][idx2][idx3][1],
        payoff[idx1][idx2][idx3][2]
    };
}

void GameMatrix::setDefaultMatrix() {
    // C C C => 7 7 7
    payoff[0][0][0][0] = 7; payoff[0][0][0][1] = 7; payoff[0][0][0][2] = 7;
    
    // C C D => 3 3 9
    payoff[0][0][1][0] = 3; payoff[0][0][1][1] = 3; payoff[0][0][1][2] = 9;
    
    // C D C => 3 9 3
    payoff[0][1][0][0] = 3; payoff[0][1][0][1] = 9; payoff[0][1][0][2] = 3;
    
    // D C C => 9 3 3
    payoff[1][0][0][0] = 9; payoff[1][0][0][1] = 3; payoff[1][0][0][2] = 3;
    
    // C D D => 0 5 5
    payoff[0][1][1][0] = 0; payoff[0][1][1][1] = 5; payoff[0][1][1][2] = 5;
    
    // D C D => 5 0 5
    payoff[1][0][1][0] = 5; payoff[1][0][1][1] = 0; payoff[1][0][1][2] = 5;
    
    // D D C => 5 5 0
    payoff[1][1][0][0] = 5; payoff[1][1][0][1] = 5; payoff[1][1][0][2] = 0;
    
    // D D D => 1 1 1
    payoff[1][1][1][0] = 1; payoff[1][1][1][1] = 1; payoff[1][1][1][2] = 1;
    
    matrixLoaded = false;
}

void GameMatrix::printMatrix() const {
    std::cout << "=== Game Matrix ===" << std::endl;
    if (matrixLoaded) {
        std::cout << "(Loaded from file)" << std::endl;
    } else {
        std::cout << "(Default matrix)" << std::endl;
    }
    
    std::cout << "C C C => " << payoff[0][0][0][0] << " " 
              << payoff[0][0][0][1] << " " << payoff[0][0][0][2] << std::endl;
    std::cout << "C C D => " << payoff[0][0][1][0] << " " 
              << payoff[0][0][1][1] << " " << payoff[0][0][1][2] << std::endl;
    std::cout << "C D C => " << payoff[0][1][0][0] << " " 
              << payoff[0][1][0][1] << " " << payoff[0][1][0][2] << std::endl;
    std::cout << "D C C => " << payoff[1][0][0][0] << " " 
              << payoff[1][0][0][1] << " " << payoff[1][0][0][2] << std::endl;
    std::cout << "C D D => " << payoff[0][1][1][0] << " " 
              << payoff[0][1][1][1] << " " << payoff[0][1][1][2] << std::endl;
    std::cout << "D C D => " << payoff[1][0][1][0] << " " 
              << payoff[1][0][1][1] << " " << payoff[1][0][1][2] << std::endl;
    std::cout << "D D C => " << payoff[1][1][0][0] << " " 
              << payoff[1][1][0][1] << " " << payoff[1][1][0][2] << std::endl;
    std::cout << "D D D => " << payoff[1][1][1][0] << " " 
              << payoff[1][1][1][1] << " " << payoff[1][1][1][2] << std::endl;
    std::cout << "===================" << std::endl;
}