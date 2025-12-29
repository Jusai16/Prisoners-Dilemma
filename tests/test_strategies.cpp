#include <gtest/gtest.h>
#include "core/Strategy.h"
#include "strategies/basic/AlwaysCooperate.h"
#include "strategies/basic/AlwaysDefect.h"
#include "strategies/basic/Random.h"
#include "strategies/advanced/FiftyFifty.h"
#include "strategies/advanced/TitForTat.h"
#include "strategies/advanced/AdaptiveStrategy.h"

// Тесты для AlwaysCooperate
TEST(StrategyTests, AlwaysCooperateTest) {
    AlwaysCooperate strategy;
    std::vector<Move> ownHistory;
    std::vector<std::vector<Move>> opponentsHistory;
    
    // Всегда должен возвращать COOPERATE
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(strategy.makeMove(ownHistory, opponentsHistory), Move::COOPERATE);
        ownHistory.push_back(Move::COOPERATE);
        opponentsHistory.push_back({Move::COOPERATE});
    }
    
    EXPECT_EQ(strategy.getName(), "AlwaysCooperate");
}

// Тесты для AlwaysDefect
TEST(StrategyTests, AlwaysDefectTest) {
    AlwaysDefect strategy;
    std::vector<Move> ownHistory;
    std::vector<std::vector<Move>> opponentsHistory;
    
    // Всегда должен возвращать DEFECT
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(strategy.makeMove(ownHistory, opponentsHistory), Move::DEFECT);
        ownHistory.push_back(Move::DEFECT);
        opponentsHistory.push_back({Move::DEFECT});
    }
    
    EXPECT_EQ(strategy.getName(), "AlwaysDefect");
}

// Тесты для FiftyFifty
TEST(StrategyTests, FiftyFiftyTest) {
    FiftyFifty strategy;
    std::vector<Move> ownHistory;
    std::vector<std::vector<Move>> opponentsHistory;
    
    // Первый ход должен быть COOPERATE (по умолчанию)
    Move firstMove = strategy.makeMove(ownHistory, opponentsHistory);
    EXPECT_EQ(firstMove, Move::COOPERATE);
    ownHistory.push_back(firstMove);
    
    // Второй ход должен быть противоположным
    Move secondMove = strategy.makeMove(ownHistory, opponentsHistory);
    EXPECT_EQ(secondMove, Move::DEFECT);
    ownHistory.push_back(secondMove);
    
    // Третий ход снова должен быть противоположным
    Move thirdMove = strategy.makeMove(ownHistory, opponentsHistory);
    EXPECT_EQ(thirdMove, Move::COOPERATE);
    
    EXPECT_EQ(strategy.getName(), "FiftyFifty");
}

// Тесты для TitForTat
TEST(StrategyTests, TitForTatTest) {
    TitForTat strategy;
    std::vector<Move> ownHistory;
    std::vector<std::vector<Move>> opponentsHistory;
    
    // Первый ход должен быть COOPERATE (по умолчанию)
    Move move1 = strategy.makeMove(ownHistory, opponentsHistory);
    EXPECT_EQ(move1, Move::COOPERATE);
    ownHistory.push_back(move1);
    
    // Если оппоненты сотрудничали - продолжаем сотрудничать
    opponentsHistory = {{Move::COOPERATE}, {Move::COOPERATE}};
    Move move2 = strategy.makeMove(ownHistory, opponentsHistory);
    EXPECT_EQ(move2, Move::COOPERATE);
    ownHistory.push_back(move2);
    
    // Если большинство оппонентов предали - предаем
    opponentsHistory = {{Move::DEFECT}, {Move::DEFECT}};
    Move move3 = strategy.makeMove(ownHistory, opponentsHistory);
    EXPECT_EQ(move3, Move::DEFECT);
    
    EXPECT_EQ(strategy.getName(), "TitForTat");
}

// Тесты для Random
TEST(StrategyTests, RandomTest) {
    RandomStrategy strategy;
    std::vector<Move> ownHistory;
    std::vector<std::vector<Move>> opponentsHistory;
    
    // Проверяем, что стратегия возвращает либо COOPERATE, либо DEFECT
    for (int i = 0; i < 20; ++i) {
        Move move = strategy.makeMove(ownHistory, opponentsHistory);
        EXPECT_TRUE(move == Move::COOPERATE || move == Move::DEFECT);
        ownHistory.push_back(move);
    }
    
    EXPECT_EQ(strategy.getName(), "Random");
}

// Тесты для AdaptiveStrategy
TEST(StrategyTests, AdaptiveStrategyTest) {
    AdaptiveStrategy strategy;
    std::vector<Move> ownHistory;
    std::vector<std::vector<Move>> opponentsHistory;
    
    // Первый ход (случайный с вероятностью cooperationLevel)
    Move move1 = strategy.makeMove(ownHistory, opponentsHistory);
    EXPECT_TRUE(move1 == Move::COOPERATE || move1 == Move::DEFECT);
    ownHistory.push_back(move1);
    
    // Добавляем историю оппонентов
    opponentsHistory = {{Move::COOPERATE, Move::COOPERATE, Move::COOPERATE},
                        {Move::COOPERATE, Move::COOPERATE, Move::COOPERATE}};
    
    // Второй ход
    Move move2 = strategy.makeMove(ownHistory, opponentsHistory);
    EXPECT_TRUE(move2 == Move::COOPERATE || move2 == Move::DEFECT);
    
    EXPECT_EQ(strategy.getName(), "AdaptiveStrategy");
}