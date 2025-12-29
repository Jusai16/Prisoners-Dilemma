#ifndef FIFTYFIFTY_H
#define FIFTYFIFTY_H

#include "core/Strategy.h"
#include <vector>
#include <string>

class FiftyFifty : public Strategy {
private:
    std::string name;
    bool lastMoveRandom;
    char firstMove;  // 'C' или 'D'
    
public:
    FiftyFifty();
    
    Move makeMove(const std::vector<Move>& ownHistory,
                  const std::vector<std::vector<Move>>& opponentsHistory) override;
    
    std::string getName() const override;
    
    void loadConfig(const std::string& configDir) override;
    
    bool isLastMoveRandom() const;
};

#endif 