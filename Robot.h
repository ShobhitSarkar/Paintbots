#ifndef ROBOT_H
#define ROBOT_H

#include "IRobotAgent.h"
#include "RobotAgentRoster.h"
#include <cstring>  
#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {
    class Robot {
    private:
        // Strategy (robot agent)
        IRobotAgent* strategy;

        // Robot's color
        RobotMoveRequest::RobotColor robotColor;

        int paintBlobsLeft;

    public:
        /**
         * @brief Default constructor 
         * Creates a red robot with the "lazy" strategy from the roster
         */
        Robot();

        /**
         * @brief Set the robot's strategy (agent)
         * @param agent Pointer to the IRobotAgent to be used
         */
        void setRobotAgent(IRobotAgent* agent);

        /**
         * @brief Get the current robot agent
         * @return Pointer to the current IRobotAgent
         */
        IRobotAgent* getRobotAgent();

        /**
         * @brief Set the robot's color
         * @param color The color to set for the robot
         */
        void setColor(RobotMoveRequest::RobotColor color);

        /**
         * @brief Get the robot's current color
         * @return The robot's color
         */
        RobotMoveRequest::RobotColor getRobotColor() const;

        /**
         * @brief Destructor to clean up the strategy if dynamically allocated
         */
        ~Robot();


        int getPaintBlobsLeft() const { return paintBlobsLeft; }

        void setPaintBlobsLeft(int count) { paintBlobsLeft = count; }
    };
}

#endif 