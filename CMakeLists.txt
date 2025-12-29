cmake_minimum_required(VERSION 3.15)
project(PrisonersDilemma)

set(VCPKG_TARGET_TRIPLET x64-windows)
set(CMAKE_TOOLCHAIN_FILE "C://Users//driuk//vcpkg//scripts//buildsystems//vcpkg.cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основное приложение
add_executable(prisoners_dilemma 
    src/main.cpp
    src/core/Game.cpp
    src/core/GameMatrix.cpp
    src/core/Tournament.cpp
    src/core/StrategyFactory.cpp
    src/core/History.cpp
    src/utils/ConfigFileParser.cpp
    src/utils/Logger.cpp
    src/utils/Parser.cpp
    src/strategies/basic/AlwaysCooperate.cpp
    src/strategies/basic/AlwaysDefect.cpp
    src/strategies/basic/Random.cpp
    src/strategies/advanced/AdaptiveStrategy.cpp
    src/strategies/advanced/TitForTat.cpp
    src/strategies/advanced/FiftyFifty.cpp
)

target_include_directories(prisoners_dilemma PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Тесты
enable_testing()

find_package(GTest REQUIRED)

# Создаем библиотеку из исходников
add_library(game_lib STATIC
    src/core/Game.cpp
    src/core/GameMatrix.cpp
    src/core/Tournament.cpp
    src/core/StrategyFactory.cpp
    src/core/History.cpp
    src/utils/ConfigFileParser.cpp
    src/utils/Logger.cpp
    src/utils/Parser.cpp
    src/strategies/basic/AlwaysCooperate.cpp
    src/strategies/basic/AlwaysDefect.cpp
    src/strategies/basic/Random.cpp
    src/strategies/advanced/AdaptiveStrategy.cpp
    src/strategies/advanced/TitForTat.cpp
    src/strategies/advanced/FiftyFifty.cpp
)

target_include_directories(game_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Тестовый исполняемый файл - только тесты
add_executable(run_tests
    tests/test_main.cpp
    tests/test_strategies.cpp
    tests/test_game.cpp
    tests/test_factory.cpp
    tests/test_config.cpp
)

target_include_directories(run_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

target_link_libraries(run_tests PRIVATE 
    game_lib
    GTest::gtest 
    GTest::gtest_main
)

# Также линкуем библиотеку к основному приложению
target_link_libraries(prisoners_dilemma PRIVATE game_lib)

# Добавляем тест
add_test(NAME PrisonersDilemmaTests COMMAND run_tests)