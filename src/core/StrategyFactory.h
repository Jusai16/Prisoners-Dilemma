#ifndef STRATEGYFACTORY_H
#define STRATEGYFACTORY_H

#include <memory>
#include <string>
#include <map>
#include <functional>
#include "core/Strategy.h"

class StrategyFactory {
private:
    std::map<std::string, std::function<std::unique_ptr<Strategy>()>> creators;
    StrategyFactory();
    StrategyFactory(const StrategyFactory&) = delete;
    StrategyFactory& operator=(const StrategyFactory&) = delete;
public:
    static StrategyFactory& getInstance();

    void registerStrategy(const std::string& neme, std::function<std::unique_ptr<Strategy>()> creator);
    std::unique_ptr<Strategy> create(const std::string& name, const std::string& configDir = "") const;
    bool exists(const std::string& name) const;
    std::vector<std::string> getAvailableStrategies() const;
    void registerAllStrategies();
};

#endif