#include "utils/ConfigFileParser.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>

namespace {
    // Вспомогательная функция для trim
    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, last - first + 1);
    }
    
    // Вспомогательная функция для toLower
    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), 
                      [](unsigned char c) { return std::tolower(c); });
        return result;
    }
}

bool ConfigFileParser::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Убираем комментарии
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }
        
        line = trim(line);
        if (line.empty()) continue;
        
        // Парсим key=value
        size_t equalsPos = line.find('=');
        if (equalsPos != std::string::npos) {
            std::string key = trim(line.substr(0, equalsPos));
            std::string value = trim(line.substr(equalsPos + 1));
            
            if (!key.empty()) {
                configMap[key] = value;
            }
        }
    }
    
    file.close();
    return true;
}

bool ConfigFileParser::loadFromDir(const std::string& configDir, const std::string& strategyName) {
    if (configDir.empty() || strategyName.empty()) return false;
    std::string filename = configDir + "/" + strategyName + ".cfg";
    return load(filename);
}

std::string ConfigFileParser::getString(const std::string& key, const std::string& defaultValue) const {
    auto it = configMap.find(key);
    return (it != configMap.end()) ? it->second : defaultValue;
}

int ConfigFileParser::getInt(const std::string& key, int defaultValue) const {
    auto it = configMap.find(key);
    if (it != configMap.end()) {
        try { 
            return std::stoi(it->second); 
        }
        catch (...) {
            return defaultValue;
        }
    }
    return defaultValue;
}

double ConfigFileParser::getDouble(const std::string& key, double defaultValue) const {
    auto it = configMap.find(key);
    if (it != configMap.end()) {
        try { 
            return std::stod(it->second); 
        }
        catch (...) {
            return defaultValue;
        }
    }
    return defaultValue;
}

bool ConfigFileParser::getBool(const std::string& key, bool defaultValue) const {
    auto it = configMap.find(key);
    if (it != configMap.end()) {
        std::string val = toLower(it->second);
        return (val == "true" || val == "1" || val == "yes");
    }
    return defaultValue;
}

bool ConfigFileParser::hasKey(const std::string& key) const {
    return configMap.find(key) != configMap.end();
}