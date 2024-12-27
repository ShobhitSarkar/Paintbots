// GameLogic.h
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "GameBoard.h"
#include "Config.h"
#include "Robot.h"
#include "RobotAgentRoster.h"
#include "PlainDisplay.h"
#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <cstring>  

#include <stdexcept>  
#include <algorithm> 


namespace Paintbots {
    class GameLogic {
    private:
        // Game configuration and components
        Config& config;
        GameBoard* board;
        Robot redRobot;
        Robot blueRobot;
        PlainDisplay display;
        bool pauseMode;
        
        // Constants
        static const int MAX_MOVES = 300;

        /**
         * Load robot strategies from configuration file
         * @param robotConfigFile Path to robot configuration file
         * @throws std::runtime_error if file cannot be opened or strategies cannot be loaded
         */
        void loadRobotStrategies(const std::string& robotConfigFile);

        /**
         * Handle pause mode between moves
         */
        void checkPause();

        /**
         * Process robot moves and update game state
         * @param redMove Red robot's move request
         * @param blueMove Blue robot's move request
         * @return true if game should continue, false if game should end
         */
        bool processRobotMoves(RobotMoveRequest* redMove, RobotMoveRequest* blueMove);

        /**
         * Clean up scan data
         * @param scan Pointer to scan array
         * @param size Size of the array
         */
        void cleanupScan(ExternalBoardSquare** scan, int size);

        /**
         * Check for game-ending conditions
         * @return true if game should end
         */
        bool checkGameEnd() const;

        /**
         * Announce game results
         */
        void announceResults() const;

    public:
        /**
         * Constructor
         * Initializes game components and loads default configuration
         */
        GameLogic();

        /**
         * Play the game
         * @param boardConfig Path to board configuration file
         * @param robotConfig Path to robot configuration file
         */
        void playGame(const std::string& boardConfig, const std::string& robotConfig);

        /**
         * Set pause mode
         * @param pausemode Whether to pause between moves
         */
        void setPause(bool pausemode);

        /**
         * Run the game from command line arguments
         * @param argc Argument count
         * @param argv Argument values
         * @return Exit status
         */
        static int runGame(int argc, char* argv[]);
    };
}

#endif
