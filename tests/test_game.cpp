#include <gtest/gtest.h>
#include "core/Game.h"
#include "core/GameMatrix.h"
#include "strategies/basic/AlwaysCooperate.h"
#include "strategies/basic/AlwaysDefect.h"

// Тесты GameMatrix
TEST(GameMatrixTests, DefaultMatrixTest) {
    GameMatrix matrix;
    
    // Проверяем несколько комбинаций
    std::vector<int> scores;
    
    // C C C => 7 7 7
    scores = matrix.getPayoff(Move::COOPERATE, Move::COOPERATE, Move::COOPERATE);
    EXPECT_EQ(scores[0], 7);
    EXPECT_EQ(scores[1], 7);
    EXPECT_EQ(scores[2], 7);
    
    // C C D => 3 3 9
    scores = matrix.getPayoff(Move::COOPERATE, Move::COOPERATE, Move::DEFECT);
    EXPECT_EQ(scores[0], 3);
    EXPECT_EQ(scores[1], 3);
    EXPECT_EQ(scores[2], 9);
    
    // D D D => 1 1 1
    scores = matrix.getPayoff(Move::DEFECT, Move::DEFECT, Move::DEFECT);
    EXPECT_EQ(scores[0], 1);
    EXPECT_EQ(scores[1], 1);
    EXPECT_EQ(scores[2], 1);
}

TEST(GameMatrixTests, LoadFromFileTest) {
    // Создадим временный файл с матрицей
    std::ofstream testFile("test_matrix.txt");
    testFile << "# Test matrix file\n";
    testFile << "C C C 10 10 10\n";
    testFile << "C C D 5 5 15\n";
    testFile << "D D D 2 2 2\n";
    testFile.close();
    
    GameMatrix matrix("test_matrix.txt");
    
    // Проверяем загруженные значения
    std::vector<int> scores = matrix.getPayoff(Move::COOPERATE, Move::COOPERATE, Move::COOPERATE);
    EXPECT_EQ(scores[0], 10);
    EXPECT_EQ(scores[1], 10);
    EXPECT_EQ(scores[2], 10);
    
    // Удаляем временный файл
    std::remove("test_matrix.txt");
}

// Тесты Game
TEST(GameTests, GameCreationTest) {
    Game game(100);
    
    EXPECT_EQ(game.getTotalRounds(), 100);
    EXPECT_EQ(game.getCurrentRound(), 0);
    EXPECT_FALSE(game.isReady());
}

TEST(GameTests, AddPlayersTest) {
    Game game(50);
    
    EXPECT_FALSE(game.isReady());
    
    // Добавляем игроков
    game.addPlayer(std::make_unique<AlwaysCooperate>());
    game.addPlayer(std::make_unique<AlwaysDefect>());
    game.addPlayer(std::make_unique<AlwaysCooperate>());
    
    EXPECT_TRUE(game.isReady());
    EXPECT_EQ(game.getPlayerNames().size(), 3);
}

TEST(GameTests, SingleRoundTest) {
    Game game(10);
    
    game.addPlayer(std::make_unique<AlwaysCooperate>());
    game.addPlayer(std::make_unique<AlwaysDefect>());
    game.addPlayer(std::make_unique<AlwaysCooperate>());
    
    EXPECT_TRUE(game.isReady());
    
    // Играем один раунд
    game.playRound();
    
    EXPECT_EQ(game.getCurrentRound(), 1);
    
    // Получаем результаты
    auto scores = game.getScores();
    EXPECT_EQ(scores.size(), 3);
    
    // C D C => 3 9 3 (по матрице из задания)
    // Первый AlwaysCooperate против AlwaysDefect и AlwaysCooperate
    EXPECT_EQ(scores[0], 3);  // Всегда сотрудничает
    EXPECT_EQ(scores[1], 9);  // Всегда предает  
    EXPECT_EQ(scores[2], 3);  // Всегда сотрудничает
}

TEST(GameTests, FullGameTest) {
    Game game(5);
    
    game.addPlayer(std::make_unique<AlwaysCooperate>());
    game.addPlayer(std::make_unique<AlwaysDefect>());
    game.addPlayer(std::make_unique<AlwaysCooperate>());
    
    // Играем все раунды
    game.playGame();
    
    EXPECT_EQ(game.getCurrentRound(), 5);
    
    auto scores = game.getScores();
    // За 5 раундов с матрицей C D C => 3 9 3
    EXPECT_EQ(scores[0], 3 * 5);  // 15
    EXPECT_EQ(scores[1], 9 * 5);  // 45
    EXPECT_EQ(scores[2], 3 * 5);  // 15
}

TEST(GameTests, ResetGameTest) {
    Game game(10);
    
    game.addPlayer(std::make_unique<AlwaysCooperate>());
    game.addPlayer(std::make_unique<AlwaysDefect>());
    game.addPlayer(std::make_unique<AlwaysCooperate>());
    
    game.playGame();
    EXPECT_EQ(game.getCurrentRound(), 10);
    
    game.reset();
    EXPECT_EQ(game.getCurrentRound(), 0);
    EXPECT_FALSE(game.isReady());
}