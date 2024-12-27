// GameLogic.cpp
#include "GameLogic.h"
#include "LazyRobot.h"     
#include "AntiRandom.h"
#include "RandomRobot.h"   
#include <stdexcept>
#include <memory>
#include <cstring>
  
#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {
    
    /**
     * Constructor for creating the board 
     */
    GameLogic::GameLogic() : 
        config(Config::getInstance()),
        board(GameBoard::getInstance("xyzzy")),
        redRobot(),
        blueRobot(),
        pauseMode(true) 
    {
        std::cout << "Starting GameLogic initialization..." << std::endl;
        board->addObserver(&display);
        
        // Initialize roster with robots
        RobotAgentRoster& roster = RobotAgentRoster::getInstance();
        std::cout << "Initial roster size: " << roster.size() << std::endl;
        
        try {
            // Create and add robots to roster
            LazyRobot* lazyRobot = new LazyRobot();
            RandomRobot* randomRobot = new RandomRobot();
            AntiRandom* antiRandom = new AntiRandom();  // Add this line
            
            std::cout << "Created robot instances" << std::endl;
            
            roster.add(lazyRobot);
            roster.add(randomRobot);
            roster.add(antiRandom);  // Add this line
            
            std::cout << "Added robots to roster. New size: " << roster.size() << std::endl;

            // Debug check
            try {
                IRobotAgent* test = roster["LazyRobot"];
                std::cout << "Successfully retrieved LazyRobot from roster" << std::endl;
            } catch (const std::exception& e) {
                std::cout << "Failed to retrieve LazyRobot: " << e.what() << std::endl;
            }
            
        } catch (const std::exception& e) {
            std::cerr << "Failed to initialize robots: " << e.what() << std::endl;
            throw;
        }
    }

    /**
     * Loads the different robot strategies - lazy, random or antirandom 
     */
    void GameLogic::loadRobotStrategies(const std::string& robotConfigFile) {
        std::cout << "Loading robot strategies from: " << robotConfigFile << std::endl;
        
        std::ifstream configFile(robotConfigFile);
        if (!configFile.is_open()) {
            throw std::runtime_error("Could not open robot configuration file: " + robotConfigFile);
        }

        std::string redRobotName, blueRobotName;
        std::getline(configFile, redRobotName);
        std::getline(configFile, blueRobotName);
        
        redRobotName.erase(0, redRobotName.find_first_not_of(" \t\n\r"));
        redRobotName.erase(redRobotName.find_last_not_of(" \t\n\r") + 1);
        blueRobotName.erase(0, blueRobotName.find_first_not_of(" \t\n\r"));
        blueRobotName.erase(blueRobotName.find_last_not_of(" \t\n\r") + 1);

        std::cout << "Looking for robots: Red='" << redRobotName << "', Blue='" << blueRobotName << "'" << std::endl;
        
        RobotAgentRoster& roster = RobotAgentRoster::getInstance();
        std::cout << "Current roster size: " << roster.size() << std::endl;

        try {
            for (size_t i = 0; i < roster.size(); ++i) {
                std::cout << "Robot " << i << ": " << roster[i]->getRobotName() << std::endl;
            }
            
            // Set red robot
            IRobotAgent* redStrategy = roster[redRobotName];
            redRobot.setRobotAgent(redStrategy);
            redRobot.setColor(RobotMoveRequest::RED);
            
            // Set blue robot
            IRobotAgent* blueStrategy = roster[blueRobotName];
            blueRobot.setRobotAgent(blueStrategy);
            blueRobot.setColor(RobotMoveRequest::BLUE);
        } catch (const std::exception& e) {
            throw std::runtime_error("Error setting robot strategies: " + std::string(e.what()));
        }
    }

    /**
     * Pauses execution of game, which may continue when the user hits enter 
     */
    void GameLogic::checkPause() {
        if (pauseMode) {
            std::cout << "Press Enter to continue...";
            std::cin.get();
        }
    }

    /**
     * Processes the different robot moves 
     */
    bool GameLogic::processRobotMoves(RobotMoveRequest* redMove, RobotMoveRequest* blueMove) {
        if (!redMove || !blueMove) {
            throw std::runtime_error("Invalid move requests");
        }

        // Process paint blob hits first
        bool redHit = board->paintBlobHit(*redMove);
        bool blueHit = board->paintBlobHit(*blueMove);

        if (redHit) {
            std::cout << "Red robot's paint blob hit the blue robot!" << std::endl;
        }
        if (blueHit) {
            std::cout << "Blue robot's paint blob hit the red robot!" << std::endl;
        }

        // Perform moves
        bool redMoveResult = board->moveRobot(*redMove);
        bool blueMoveResult = board->moveRobot(*blueMove);

        // Check for invalid moves or collisions
        if (!redMoveResult || !blueMoveResult) {
            std::cout << "Game ended due to invalid move.\n";
            return false;
        }

        return !checkGameEnd();
    }

    /**
     * Method to clean up scan contents 
     */
    void GameLogic::cleanupScan(ExternalBoardSquare** scan, int size) {
        if (scan) {
            for (int i = 0; i < size; ++i) {
                delete[] scan[i];
            }
            delete[] scan;
        }
    }

    /**
     * Method to check for game ending conditions 
     */
    bool GameLogic::checkGameEnd() const {
        for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
            for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
                if (board->getSquare(i, j).redRobotPresent() && 
                    board->getSquare(i, j).blueRobotPresent()) {
                    std::cout << "Robots collided!\n";
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * Method to print out results of the game 
     */
    void GameLogic::announceResults() const {
        int redScore = board->redScore();
        int blueScore = board->blueScore();

        std::cout << "\nGame Over!\n";
        std::cout << "Red Score: " << redScore << "\n";
        std::cout << "Blue Score: " << blueScore << "\n";

        if (redScore > blueScore) {
            std::cout << "Red Robot Wins!\n";
        }
        else if (blueScore > redScore) {
            std::cout << "Blue Robot Wins!\n";
        }
        else {
            std::cout << "It's a Tie!\n";
        }
    }

    /**
     * Executes game 
     */
    void GameLogic::playGame(const std::string& boardConfig, const std::string& robotConfig) {
        try {
            // Load configurations
            Config::getInstance(boardConfig);
            loadRobotStrategies(robotConfig);

            // Initial board display
            display.display();
            checkPause();

            // Main game loop 
            for (int moveCount = 0; moveCount < MAX_MOVES; ++moveCount) {
                // Get scans for both robots
                std::unique_ptr<ExternalBoardSquare*[]> redSrs(board->getShortRangeScan(RobotMoveRequest::RED));
                std::unique_ptr<ExternalBoardSquare*[]> redLrs(board->getLongRangeScan());
                std::unique_ptr<ExternalBoardSquare*[]> blueSrs(board->getShortRangeScan(RobotMoveRequest::BLUE));
                std::unique_ptr<ExternalBoardSquare*[]> blueLrs(board->getLongRangeScan());

                // Get move requests
                std::unique_ptr<RobotMoveRequest> redMove(redRobot.getRobotAgent()->getMove(redSrs.get(), redLrs.get()));
                std::unique_ptr<RobotMoveRequest> blueMove(blueRobot.getRobotAgent()->getMove(blueSrs.get(), blueLrs.get()));

                // Process moves
                if (!processRobotMoves(redMove.get(), blueMove.get())) {
                    break;
                }

                // Display updated board
                display.display();
                checkPause();

                // Cleanup scans
                for (int i = 0; i < 5; ++i) {
                    delete[] redSrs[i];
                    delete[] blueSrs[i];
                }
                for (int i = 0; i < GameBoard::BOARD_SIZE + 2; ++i) {
                    delete[] redLrs[i];
                    delete[] blueLrs[i];
                }
            }

            announceResults();
        }
        catch (const std::exception& e) {
            std::cerr << "Game error: " << e.what() << std::endl;
            throw;
        }
    }

    /**
     * Setter for entering pause mode 
     */
    void GameLogic::setPause(bool pausemode) {
        pauseMode = pausemode;
    }

    /**
     * Starts game execution 
     */
    int GameLogic::runGame(int argc, char* argv[]) {
        if (argc != 3) {
            std::cerr << "Usage: " << argv[0] << " <board_config_file> <robot_config_file>\n";
            return 1;
        }

        try {
            GameLogic game;
            game.playGame(argv[1], argv[2]);
            return 0;
        }
        catch (const std::exception& e) {
            std::cerr << "Game initialization error: " << e.what() << std::endl;
            return 1;
        }
    }
}

// Main function
int main(int argc, char* argv[]) {
    return Paintbots::GameLogic::runGame(argc, argv);
}