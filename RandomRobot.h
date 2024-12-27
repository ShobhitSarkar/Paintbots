#ifndef RANDOM_ROBOT_H
#define RANDOM_ROBOT_H

#include "IRobotAgent.h"
#include <random>
#include <ctime>
#include <cstring>  
#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {
    /**
     * RandomRobot class determines its actions using a combination of random number generator 
     * and basic game state analysis. 
     */
    class RandomRobot : public IRobotAgent {
        private:

            /**
             * The robot's assigned color 
             */
            RobotMoveRequest::RobotColor robotColor;

            /**
             * Random number generator 
             */
            std::mt19937 rng;

            /**
             * gets a random move
             */
            RobotMoveRequest::RobotMove getRandomMove(ExternalBoardSquare** srs);

            /**
             * The number of paint blobs left for shooting 
             */
            int paintBlobsLeft;  

            /**
             * Tracks the number of moves made by the robot 
             */
            int moveCount;  

            /**
             * Stores the last move performed by the robot 
             */
            RobotMoveRequest::RobotMove lastMove;  

            /**
             * Variable to keep track of the direction of the robot 
             */
            Direction currentDirection;  

        public:

            /**
             * Constructor 
             */
            RandomRobot();

            /**
             * Returns a string containing the robot's name 
             */
            std::string getRobotName() override;

            /**
             * retrieves the name of the robot's creator 
             */
            std::string getRobotCreator() override;

            /**
             * Determines robot's next move based on game board state 
             */
            RobotMoveRequest* getMove(ExternalBoardSquare** srs, 
                                    ExternalBoardSquare** lrs) override;

            /**
             * Assigns the robot's color 
             */
            void setRobotColor(RobotMoveRequest::RobotColor c) override;
    };
}

#endif