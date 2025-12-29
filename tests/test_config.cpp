#include <gtest/gtest.h>
#include "utils/ConfigFileParser.h"
#include "utils/Parser.h"
#include <filesystem>

// Тесты ConfigFileParser
TEST(ConfigFileParserTests, BasicParsingTest) {
    ConfigFileParser parser;
    
    // Создаем временный конфигурационный файл
    std::ofstream testFile("test_config.cfg");
    testFile << "# Test configuration\n";
    testFile << "name=TestStrategy\n";
    testFile << "value=42\n";
    testFile << "rate=3.14\n";
    testFile << "enabled=true\n";
    testFile.close();
    
    EXPECT_TRUE(parser.load("test_config.cfg"));
    
    // Проверяем значения
    EXPECT_EQ(parser.getString("name"), "TestStrategy");
    EXPECT_EQ(parser.getString("nonexistent", "default"), "default");
    
    EXPECT_EQ(parser.getInt("value", 0), 42);
    EXPECT_EQ(parser.getInt("nonexistent", 99), 99);
    
    EXPECT_DOUBLE_EQ(parser.getDouble("rate", 0.0), 3.14);
    EXPECT_DOUBLE_EQ(parser.getDouble("nonexistent", 1.0), 1.0);
    
    EXPECT_TRUE(parser.getBool("enabled", false));
    EXPECT_FALSE(parser.getBool("nonexistent", false));
    
    // Проверяем наличие ключей
    EXPECT_TRUE(parser.hasKey("name"));
    EXPECT_FALSE(parser.hasKey("nonexistent"));
    
    // Удаляем временный файл
    std::remove("test_config.cfg");
}

TEST(ConfigFileParserTests, LoadFromDirTest) {
    ConfigFileParser parser;
    
    // Создаем временную директорию и файл
    std::filesystem::create_directory("test_configs");
    std::ofstream testFile("test_configs/test_strategy.cfg");
    testFile << "param1=value1\n";
    testFile << "param2=100\n";
    testFile.close();
    
    EXPECT_TRUE(parser.loadFromDir("test_configs", "test_strategy"));
    EXPECT_TRUE(parser.hasKey("param1"));
    EXPECT_EQ(parser.getString("param1"), "value1");
    EXPECT_EQ(parser.getInt("param2", 0), 100);
    
    // Очистка
    std::remove("test_configs/test_strategy.cfg");
    std::filesystem::remove("test_configs");
}

TEST(ConfigFileParserTests, InvalidFileTest) {
    ConfigFileParser parser;
    
    // Пытаемся загрузить несуществующий файл
    EXPECT_FALSE(parser.load("nonexistent_file.cfg"));
    EXPECT_FALSE(parser.loadFromDir("nonexistent_dir", "strategy"));
}

// Тесты Parser (аргументы командной строки)
TEST(ParserTests, BasicArgumentsTest) {
    const char* argv[] = {
        "program",
        "strategy1",
        "strategy2", 
        "strategy3",
        "--mode=fast",
        "--steps=50",
        "--configs=./my_configs",
        "--matrix=matrix.txt"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    Parser parser(argc, const_cast<char**>(argv));
    
    EXPECT_EQ(parser.getStrategies().size(), 3);
    EXPECT_EQ(parser.getStrategies()[0], "strategy1");
    EXPECT_EQ(parser.getStrategies()[1], "strategy2");
    EXPECT_EQ(parser.getStrategies()[2], "strategy3");
    
    EXPECT_EQ(parser.getMode(), "fast");
    EXPECT_EQ(parser.getSteps(), 50);
    EXPECT_EQ(parser.getConfigDir(), "./my_configs");
    EXPECT_EQ(parser.getMatrixFile(), "matrix.txt");
}

TEST(ParserTests, DefaultValuesTest) {
    const char* argv[] = {
        "program",
        "s1",
        "s2",
        "s3"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    Parser parser(argc, const_cast<char**>(argv));
    
    EXPECT_EQ(parser.getStrategies().size(), 3);
    EXPECT_EQ(parser.getMode(), "detailed");  // Значение по умолчанию
    EXPECT_EQ(parser.getSteps(), 100);        // Значение по умолчанию
    EXPECT_EQ(parser.getConfigDir(), ".");    // Значение по умолчанию
    EXPECT_EQ(parser.getMatrixFile(), "");    // Значение по умолчанию
}

TEST(ParserTests, TournamentModeTest) {
    const char* argv[] = {
        "program",
        "s1", "s2", "s3", "s4", "s5",
        "--mode=tournament"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    Parser parser(argc, const_cast<char**>(argv));
    
    EXPECT_EQ(parser.getStrategies().size(), 5);
    EXPECT_EQ(parser.getMode(), "tournament");
}

TEST(ParserTests, AutoTournamentTest) {
    // Если больше 3 стратегий и mode не указан, должен автоматически стать tournament
    const char* argv[] = {
        "program",
        "s1", "s2", "s3", "s4"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    Parser parser(argc, const_cast<char**>(argv));
    
    EXPECT_EQ(parser.getStrategies().size(), 4);
    EXPECT_EQ(parser.getMode(), "tournament");  // Автоматически
}

TEST(ParserTests, ValidationTest) {
    // Недостаточно аргументов
    const char* argv1[] = {"program"};
    EXPECT_THROW(Parser(1, const_cast<char**>(argv1)), std::invalid_argument);
    
    // Некорректный mode
    const char* argv2[] = {"program", "s1", "s2", "s3", "--mode=invalid"};
    EXPECT_THROW(Parser(5, const_cast<char**>(argv2)), std::invalid_argument);
    
    // Некорректное количество шагов
    const char* argv3[] = {"program", "s1", "s2", "s3", "--steps=0"};
    EXPECT_THROW(Parser(5, const_cast<char**>(argv3)), std::invalid_argument);
    
    const char* argv4[] = {"program", "s1", "s2", "s3", "--steps=-10"};
    EXPECT_THROW(Parser(5, const_cast<char**>(argv4)), std::invalid_argument);
    
    // Некорректное количество стратегий для detailed/fast режима
    const char* argv5[] = {"program", "s1", "s2", "--mode=detailed"};
    EXPECT_THROW(Parser(4, const_cast<char**>(argv5)), std::invalid_argument);
    
    const char* argv6[] = {"program", "s1", "s2", "s3", "s4", "--mode=fast"};
    EXPECT_THROW(Parser(6, const_cast<char**>(argv6)), std::invalid_argument);
}