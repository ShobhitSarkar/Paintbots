// LazyRobot.h
#ifndef LAZY_ROBOT_H
#define LAZY_ROBOT_H

#include "IRobotAgent.h"
#include <random>
#include <ctime>
#include <cstring>  

#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {

    /**
     * Implementation of the IRobotAgent 
     */
    class LazyRobot : public IRobotAgent {
        private:
            RobotMoveRequest::RobotColor robotColor; // robot's assigned color (red or blue)
            std::mt19937 rng; // random number generator for decision making 
            /**
             * Generates a random direction for the robot to move 
             */
            Direction getRandomDirection();
            int moveCount;  // Tracks the number of moves made by the robot 
            int paintBlobsLeft;  // Remaining number of paint blobs available for shooting 
            Direction lastDirection;  // Tracks the last direction the robot moved in 

        public:
            /**
             * Initializes robot's attributes 
             */
            LazyRobot();

            /**
             * Returns the robot's name
             * @return A string representing the robot's name 
             */
            std::string getRobotName() override;

            /**
             * Retrieves the creator's name of the robot 
             */
            std::string getRobotCreator() override;

            /**
             * Analyzes the short-range and long-range scans in order to make a decision 
             */
            RobotMoveRequest* getMove(ExternalBoardSquare** srs, 
                                    ExternalBoardSquare** lrs) override;

            /**
             * Sets the robot's color 
             */
            void setRobotColor(RobotMoveRequest::RobotColor c) override;
    };
}

#endif
