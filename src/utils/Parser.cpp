#include "utils/Parser.h"
#include <iostream>
#include <algorithm>

Parser::Parser(int argc, char* argv[]) {
    setDefaultValues();

    if (argc < 2) {
        throw std::invalid_argument("Not enought arguments");
    }

    parseArguments(argc, argv);

    if (!validate()) {
        throw std::invalid_argument("Invalid arguments");
    }
}

void Parser::setDefaultValues() {
    mode = "detailed";
    steps = 100;
    configDir = ".";
    matrixFile = "";
}

void Parser::parseArguments(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg.substr(0, 2) == "--") {
            if (arg.substr(0, 7) == "--mode=") {
                mode = arg.substr(7);
            }
            else if (arg.substr(0, 8) == "--steps=") {
                steps = std::stoi(arg.substr(8));
            }
            else if (arg.substr(0, 10) == "--configs=") {
                configDir = arg.substr(10);
            }
            else if (arg.substr(0, 9) == "--matrix=") {
                matrixFile = arg.substr(9);
            } else if (arg == "--help"){
                
            }
            else {
                std::cerr << "Warning: Unknown option " << arg << std::endl;
            }
        }
        else {
            strategies.push_back(arg);
        }
    }

    if (strategies.size() > 3 && mode == "detailed") {
        mode = "tournament";
    }
}

bool Parser::validate() const {
    if (strategies.empty()) {
        std::cerr << "Error: No starategies specified" << std::endl;
        return false;
    }

    if (mode != "detailed" && mode != "fast" && mode != "tournament") {
        std::cerr << "Error: Invalid mode. Use: detailed, fast, or tournament" << std::endl;
        return false;
    }

    if (steps <= 0) {
        std::cerr << "Error: Steps must be positive" << std::endl;
        return false;
    }

    if ((mode == "detailed" || mode == "fast") && strategies.size() != 3) {
        std::cerr << "Error: detailed/fast mode requires exactly 3 strategies" << std::endl;
        return false;
    }

    return true;
}