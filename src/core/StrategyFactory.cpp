#include "StrategyFactory.h"
#include "strategies/basic/Random.h"
#include "strategies/basic/AlwaysCooperate.h"
#include "strategies/basic/AlwaysDefect.h"
#include "strategies/advanced/FiftyFifty.h"
#include "strategies/advanced/TitForTat.h"
#include "strategies/advanced/AdaptiveStrategy.h"
#include <algorithm>
#include <iostream>
#include <cctype>

StrategyFactory::StrategyFactory() {
    registerAllStrategies();
}

StrategyFactory& StrategyFactory::getInstance() {
    static StrategyFactory instance;
    return instance;
}

void StrategyFactory::registerStrategy(const std::string& name, std::function<std::unique_ptr<Strategy>()> creator) {
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
    
    creators[lowerName] = creator;
    
    // Регистрируем альтернативные имена
    if (lowerName == "random") {
        creators["rand"] = creator;
        creators["rnd"] = creator;
    }
    else if (lowerName == "alwayscooperate") {
        creators["always_cooperate"] = creator;
        creators["cooperate"] = creator;
        creators["coop"] = creator;
        creators["ac"] = creator;
    }
    else if (lowerName == "alwaysdefect") {
        creators["always_defect"] = creator;
        creators["defect"] = creator;
        creators["def"] = creator;
        creators["ad"] = creator;
    }
    else if (lowerName == "fiftyfifty") {
        creators["fifty_fifty"] = creator;
        creators["5050"] = creator;
        creators["ff"] = creator;
    }
    else if (lowerName == "titfortat") {
        creators["tit_for_tat"] = creator;
        creators["tft"] = creator;
        creators["toothfortooth"] = creator;
    }
    else if (lowerName == "adaptive") {
        creators["adaptive_strategy"] = creator;
        creators["adapt"] = creator;
        creators["adaptive"] = creator;
    }
}

std::unique_ptr<Strategy> StrategyFactory::create(const std::string& name, const std::string& configDir) const {
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
    
    auto it = creators.find(lowerName);
    if (it != creators.end()) {
        auto strategy = it->second();

        if(!configDir.empty()) {
            strategy->loadConfig(configDir);
        }

        return strategy;
    }
    
    std::cerr << "Error: Unknown strategy '" << name << "'" << std::endl;
    std::cerr << "Available strategies: ";
    auto strategies = getAvailableStrategies();
    for (size_t i = 0; i < strategies.size(); ++i) {
        std::cerr << strategies[i];
        if (i < strategies.size() - 1) std::cerr << ", ";
    }
    std::cerr << std::endl;
    
    return nullptr;
}

bool StrategyFactory::exists(const std::string& name) const {
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
    return creators.find(lowerName) != creators.end();
}

std::vector<std::string> StrategyFactory::getAvailableStrategies() const {
    std::vector<std::string> result;
    
    // Основные стратегии
    const std::vector<std::pair<std::string, std::vector<std::string>>> strategyGroups = {
        {"random", {"random", "rand", "rnd"}},
        {"alwayscooperate", {"alwayscooperate", "always_cooperate", "cooperate", "coop", "ac"}},
        {"alwaysdefect", {"alwaysdefect", "always_defect", "defect", "def", "ad"}},
        {"fiftyfifty", {"fiftyfifty", "fifty_fifty", "5050", "ff"}},
        {"titfortat", {"titfortat", "tit_for_tat", "tft", "toothfortooth"}},
        {"adaptive", {"adaptive", "adaptive_strategy", "adapt"}}
    };
    
    // Добавляем основные имена
    for (const auto& group : strategyGroups) {
        for (const auto& alias : group.second) {
            if (creators.find(alias) != creators.end()) {
                result.push_back(group.first);
                break;
            }
        }
    }
    
    // Удаляем дубликаты
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    
    return result;
}

void StrategyFactory::registerAllStrategies() {
    // Базовые стратегии
    registerStrategy("random", []() -> std::unique_ptr<Strategy> {
        return std::make_unique<RandomStrategy>();
    });
    
    registerStrategy("alwayscooperate", []() -> std::unique_ptr<Strategy> {
        return std::make_unique<AlwaysCooperate>();
    });
    
    registerStrategy("alwaysdefect", []() -> std::unique_ptr<Strategy> {
        return std::make_unique<AlwaysDefect>();
    });
    
    registerStrategy("fiftyfifty", []() -> std::unique_ptr<Strategy> {
        return std::make_unique<FiftyFifty>();
    });
    
    // Сложные стратегии
    registerStrategy("titfortat", []() -> std::unique_ptr<Strategy> {
        return std::make_unique<TitForTat>();
    });
    
    registerStrategy("adaptive", []() -> std::unique_ptr<Strategy> {
        return std::make_unique<AdaptiveStrategy>();
    });
}