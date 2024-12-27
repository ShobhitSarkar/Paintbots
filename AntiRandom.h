#ifndef ANTI_RANDOM_H
#define ANTI_RANDOM_H

#include "IRobotAgent.h"
#include <random>
#include <ctime>
#include <queue>
#include <cstring>  
#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {
    class AntiRandom : public IRobotAgent {
        private:
            RobotMoveRequest::RobotColor robotColor; // keep track of robot color 
            std::mt19937 rng;
            int paintBlobsLeft; // number of paintblobs left to shoot 
            int moveCount; // counting the number of moves 
            Direction currentDirection; // keeps track of the robot's current direction 
            std::queue<RobotMoveRequest::RobotMove> plannedMoves; // queue to keep track of the moves the robot is going to make 
            
            /**
             * Determines whether an enemy is in range 
             */
            bool isEnemyInRange(ExternalBoardSquare** srs, Direction& enemyDir, int& distance);

            /**
             * Determines whether a move is safe to make based on scans 
             */
            bool isSafeMove(ExternalBoardSquare** srs, RobotMoveRequest::RobotMove move);

            /**
             * Analyzes board state to predict the best move 
             */
            void analyzeBoardState(ExternalBoardSquare** lrs, int& myTerritory, int& enemyTerritory);

            /**
             * Finds the best direction to move in based on the scan 
             */
            Direction findBestDirection(ExternalBoardSquare** srs);

            /**
             * Helper method to help determine whether a robot would be able to shoot 
             */
            bool shouldShoot(Direction enemyDir);

        public:

            /**
             * Constructor 
             */
            AntiRandom();

            /**
             * Gets robot name (from base class )
             */
            std::string getRobotName() override;

            /**
             * Gets agent that created robot (from base class)
             */
            std::string getRobotCreator() override;

            /**
             * Gets the move based on different factors 
             */
            RobotMoveRequest* getMove(ExternalBoardSquare** srs, ExternalBoardSquare** lrs) override;

            /**
             * Sets robot color 
             */
            void setRobotColor(RobotMoveRequest::RobotColor c) override;
    };
}

#endif
