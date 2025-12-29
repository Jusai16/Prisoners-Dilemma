#ifndef CONFIGFILEPARSER_H
#define CONFIGFILEPARSER_H

#include <string>
#include <map>

class ConfigFileParser {
private:
    std::map<std::string, std::string> configMap;
    
public:
    ConfigFileParser() = default;
    
    // Основные методы
    bool load(const std::string& filename);
    bool loadFromDir(const std::string& configDir, const std::string& strategyName);
    
    // Методы получения значений
    std::string getString(const std::string& key, const std::string& defaultValue = "") const;
    int getInt(const std::string& key, int defaultValue = 0) const;
    double getDouble(const std::string& key, double defaultValue = 0.0) const;
    bool getBool(const std::string& key, bool defaultValue = false) const;
    
    // Проверка наличия ключа
    bool hasKey(const std::string& key) const;
};

#endif