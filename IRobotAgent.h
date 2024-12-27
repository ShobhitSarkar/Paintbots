// IRobotAgent.h
#ifndef IROBOT_AGENT_H
#define IROBOT_AGENT_H

#include "GameBoard.h"
#include <string>
#include <cstring>  

#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {

    /**
     * @class IRobotAgent 
     * 
     * Abstract class for a robot agent in the game 
     * 
     */
    class IRobotAgent {
        public:

            /**
             * Virtual destructor for the IRobotAgent interface 
             */
            virtual ~IRobotAgent() = default;

            /**
             * Should return a string represent a robot's name 
             */
            virtual std::string getRobotName() = 0;

            /**
             * Should return a string containing the creator's name 
             */
            virtual std::string getRobotCreator() = 0;

            /**
             * Determines the next move for the robot based on the short and long range scans 
             */
            virtual RobotMoveRequest* getMove(ExternalBoardSquare** srs, 
                                            ExternalBoardSquare** lrs) = 0;

            /**
             * Sets the robot's color 
             */
            virtual void setRobotColor(RobotMoveRequest::RobotColor c) = 0;
    };
}

#endif