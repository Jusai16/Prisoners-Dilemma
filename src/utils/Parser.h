#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <string>
#include <vector>
#include <map>
#include <stdexcept>

class Parser {
private:
    std::vector<std::string> strategies;
    std::string mode;
    int steps;
    std::string configDir;
    std::string matrixFile;

    void parseArguments(int argc, char* argv[]);
    void setDefaultValues();
public:
    Parser(int argc, char* argv[]);
    const std::vector<std::string>& getStrategies() const { return strategies; }
    const std::string& getMode() const { return mode; }
    int getSteps() const { return steps; }
    const std::string& getConfigDir() const { return configDir; }
    const std::string& getMatrixFile() const { return matrixFile; }

    bool validate() const;
};

#endif