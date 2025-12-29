#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <vector>
#include <map>          
#include <memory>
#include <chrono>        
#include <ctime>         
#include <iomanip>      
#include <algorithm>     
#include "core/Strategy.h"

class Logger {
private:
    std::ofstream logFile;
    bool enabled;
    std::string logPath;
    
public:
    Logger(const std::string& configDir = "");
    ~Logger();
    
    void logRound(int round, 
                  const std::vector<std::string>& playerNames,
                  const std::vector<Move>& moves,
                  const std::vector<int>& roundScores,
                  const std::vector<int>& totalScores);
    
    void logGameStart(const std::vector<std::string>& playerNames, int totalRounds);
    void logGameEnd(const std::vector<std::string>& playerNames,
                    const std::vector<int>& finalScores);
    
    void logTournamentStart(const std::vector<std::string>& allStrategies);
    void logTournamentEnd(const std::map<std::string, int>& finalScores);
    
    void setEnabled(bool enable) { enabled = enable; }
    bool isEnabled() const { return enabled; }
    
private:
    std::string moveToString(Move move) const;
    std::string getTimestamp() const;
};

#endif 