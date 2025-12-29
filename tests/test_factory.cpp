#include <gtest/gtest.h>
#include "core/StrategyFactory.h"

TEST(StrategyFactoryTests, FactoryCreationTest) {
    auto& factory = StrategyFactory::getInstance();
    
    // Проверяем создание всех стратегий
    auto alwaysCooperate = factory.create("alwayscooperate");
    EXPECT_NE(alwaysCooperate, nullptr);
    EXPECT_EQ(alwaysCooperate->getName(), "AlwaysCooperate");
    
    auto alwaysDefect = factory.create("alwaysdefect");
    EXPECT_NE(alwaysDefect, nullptr);
    EXPECT_EQ(alwaysDefect->getName(), "AlwaysDefect");
    
    auto random = factory.create("random");
    EXPECT_NE(random, nullptr);
    EXPECT_EQ(random->getName(), "RandomStrategy");
    
    auto fiftyfifty = factory.create("fiftyfifty");
    EXPECT_NE(fiftyfifty, nullptr);
    EXPECT_EQ(fiftyfifty->getName(), "FiftyFifty");
    
    auto titfortat = factory.create("titfortat");
    EXPECT_NE(titfortat, nullptr);
    EXPECT_EQ(titfortat->getName(), "TitForTat");
    
    auto adaptive = factory.create("adaptive");
    EXPECT_NE(adaptive, nullptr);
    EXPECT_EQ(adaptive->getName(), "AdaptiveStrategy");
}

TEST(StrategyFactoryTests, FactoryAliasesTest) {
    auto& factory = StrategyFactory::getInstance();
    
    // Проверяем альтернативные имена
    auto ac1 = factory.create("ac");
    EXPECT_NE(ac1, nullptr);
    EXPECT_EQ(ac1->getName(), "AlwaysCooperate");
    
    auto ac2 = factory.create("cooperate");
    EXPECT_NE(ac2, nullptr);
    EXPECT_EQ(ac2->getName(), "AlwaysCooperate");
    
    auto ad1 = factory.create("ad");
    EXPECT_NE(ad1, nullptr);
    EXPECT_EQ(ad1->getName(), "AlwaysDefect");
    
    auto ad2 = factory.create("defect");
    EXPECT_NE(ad2, nullptr);
    EXPECT_EQ(ad2->getName(), "AlwaysDefect");
    
    auto tft1 = factory.create("tft");
    EXPECT_NE(tft1, nullptr);
    EXPECT_EQ(tft1->getName(), "TitForTat");
    
    auto tft2 = factory.create("tit_for_tat");
    EXPECT_NE(tft2, nullptr);
    EXPECT_EQ(tft2->getName(), "TitForTat");
}

TEST(StrategyFactoryTests, FactoryCaseInsensitiveTest) {
    auto& factory = StrategyFactory::getInstance();
    
    // Проверяем регистронезависимость
    auto strategy1 = factory.create("ALWAYSCOOPERATE");
    EXPECT_NE(strategy1, nullptr);
    
    auto strategy2 = factory.create("AlwaysCooperate");
    EXPECT_NE(strategy2, nullptr);
    
    auto strategy3 = factory.create("alwayscooperate");
    EXPECT_NE(strategy3, nullptr);
    
    auto strategy4 = factory.create("AlWaYsCoOpErAtE");
    EXPECT_NE(strategy4, nullptr);
}

TEST(StrategyFactoryTests, FactoryInvalidStrategyTest) {
    auto& factory = StrategyFactory::getInstance();
    
    // Проверяем обработку несуществующей стратегии
    auto invalid = factory.create("nonexistent_strategy");
    EXPECT_EQ(invalid, nullptr);
}

TEST(StrategyFactoryTests, FactoryExistsTest) {
    auto& factory = StrategyFactory::getInstance();
    
    EXPECT_TRUE(factory.exists("alwayscooperate"));
    EXPECT_TRUE(factory.exists("ALWAYSDEFECT"));
    EXPECT_TRUE(factory.exists("random"));
    EXPECT_TRUE(factory.exists("FiftyFifty"));
    EXPECT_TRUE(factory.exists("TFT"));
    EXPECT_TRUE(factory.exists("adaptive"));
    
    EXPECT_FALSE(factory.exists("nonexistent"));
    EXPECT_FALSE(factory.exists(""));
}

TEST(StrategyFactoryTests, FactoryAvailableStrategiesTest) {
    auto& factory = StrategyFactory::getInstance();
    
    auto strategies = factory.getAvailableStrategies();
    
    // Проверяем, что все основные стратегии есть в списке
    EXPECT_NE(std::find(strategies.begin(), strategies.end(), "alwayscooperate"), strategies.end());
    EXPECT_NE(std::find(strategies.begin(), strategies.end(), "alwaysdefect"), strategies.end());
    EXPECT_NE(std::find(strategies.begin(), strategies.end(), "random"), strategies.end());
    EXPECT_NE(std::find(strategies.begin(), strategies.end(), "fiftyfifty"), strategies.end());
    EXPECT_NE(std::find(strategies.begin(), strategies.end(), "titfortat"), strategies.end());
    EXPECT_NE(std::find(strategies.begin(), strategies.end(), "adaptive"), strategies.end());
    
    // Проверяем, что список не пустой
    EXPECT_FALSE(strategies.empty());
}