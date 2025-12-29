#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <map>

#include "core/Game.h"
#include "core/GameMatrix.h"
#include "core/Tournament.h"
#include "core/Strategy.h"
#include "core/StrategyFactory.h"
#include "core/History.h"

#include "utils/Parser.h"
#include "utils/Logger.h"

void printHelp() {
    std::cout << "Prisoner's Dilemma (3 players)" << std::endl;
    std::cout << "===============================" << std::endl;
    std::cout << "\nUsage:" << std::endl;
    std::cout << "  prisoners_dilemma <strategy1> <strategy2> <strategy3> [options]" << std::endl;
    std::cout << "\nOptions:" << std::endl;
    std::cout << "  --mode=detailed|fast|tournament" << std::endl;
    std::cout << "  --steps=<number>" << std::endl;
    std::cout << "  --configs=<directory>" << std::endl;
    std::cout << "  --matrix=<filename>      # Load matrix from file" << std::endl;
    std::cout << "  --help                   # Show this help" << std::endl;
    std::cout << "\nExamples:" << std::endl;
    std::cout << "  prisoners_dilemma random alwayscooperate alwaysdefect" << std::endl;
    std::cout << "  prisoners_dilemma tft adaptive random --mode=fast --steps=50" << std::endl;
    std::cout << "  prisoners_dilemma random coop def --matrix=matrix.txt" << std::endl;
    std::cout << "  prisoners_dilemma s1 s2 s3 s4 s5 --mode=tournament" << std::endl;
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 2 || std::string(argv[1]) == "--help") {
            printHelp();
            return 0;
        }
        
        // Парсинг аргументов
        Parser config(argc, argv);
        
        // Создаем логгер
        Logger logger(config.getConfigDir());
        
        // Получаем фабрику стратегий
        auto& factory = StrategyFactory::getInstance();
        
        if (config.getMode() == "tournament") {
            // Турнирный режим
            std::cout << "\n=== PRISONER'S DILEMMA TOURNAMENT ===" << std::endl;
            Tournament tournament(config.getStrategies(), 
                                 config.getSteps(),
                                 config.getConfigDir(),
                                 config.getMatrixFile());
            
            // Показываем матрицу для турнира
            if (!config.getMatrixFile().empty()) {
                std::cout << "Using matrix from file: " << config.getMatrixFile() << std::endl;
            }
            
            logger.logTournamentStart(config.getStrategies());
            tournament.run();
            tournament.printResults();
            
            auto tournamentScores = tournament.getScores();
            logger.logTournamentEnd(tournamentScores);
            
        } else {
            // Обычная игра (3 стратегии)
            Game game(config.getSteps(), config.getMatrixFile());
            
            // Показываем матрицу игры
            std::cout << "\n=== PRISONER'S DILEMMA ===" << std::endl;
            if (!config.getMatrixFile().empty()) {
                std::cout << "Matrix loaded from: " << config.getMatrixFile() << std::endl;
            }
            game.printMatrix();  
            
            // Создаем и добавляем стратегии
            for (const auto& name : config.getStrategies()) {
                auto strategy = factory.create(name, config.getConfigDir());
                if (!strategy) {
                    std::cerr << "Error: Cannot create strategy '" << name << "'" << std::endl;
                    return 1;
                }
                game.addPlayer(std::move(strategy));
            }
            
            if (!game.isReady()) {
                std::cerr << "Error: Game setup failed! Need exactly 3 players." << std::endl;
                return 1;
            }
            
            // Логируем начало игры
            logger.logGameStart(game.getPlayerNames(), config.getSteps());
            
            // Информация о игре
            std::cout << "Game mode: " << config.getMode() << std::endl;
            std::cout << "Total rounds: " << config.getSteps() << std::endl;
            std::cout << "Players: ";
            auto names = game.getPlayerNames();
            for (const auto& name : names) {
                std::cout << name << " ";
            }
            std::cout << std::endl;
            std::cout << "================================\n" << std::endl;
            
            // Запускаем в нужном режиме
            if (config.getMode() == "detailed") {
                std::cout << "DETAILED MODE" << std::endl;
                std::cout << "Press Enter for next round, 'q' to quit.\n" << std::endl;
                std::cin.ignore();
                
                while (game.getCurrentRound() < game.getTotalRounds()) {
                    game.playRound();
                    game.printRoundInfo();
                    
                    std::string input;
                    std::cout << "Press Enter to continue, 'q' to quit: ";
                    std::getline(std::cin, input);
                    
                    if (input == "q" || input == "quit") {
                        std::cout << "\nGame stopped after " << game.getCurrentRound() << " rounds." << std::endl;
                        break;
                    }
                }
            } else {
                std::cout << "FAST MODE" << std::endl;
                std::cout << "Playing " << config.getSteps() << " rounds...\n" << std::endl;
                game.playGame();
            }
            
            // Выводим результаты
            game.printFinalResults();
            
            // Логируем конец игры
            logger.logGameEnd(game.getPlayerNames(), game.getScores());
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        printHelp();
        return 1;
    }
    
    return 0;
}