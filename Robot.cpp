#include "Robot.h"
#include <iostream>
#include <cstring>  
#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {

    /**
     * pointer to the robot's strategy (agent)
     */
    Robot::Robot() : 
        strategy(nullptr),
        robotColor(RobotMoveRequest::RED)
    {
        std::cout << "Creating new Robot with null strategy" << std::endl;
    }

    /**
     * Specifies whether the robot belongs to red or blue "team"
     */
    void Robot::setRobotAgent(IRobotAgent* agent) {
        if (agent == nullptr) {
            throw std::invalid_argument("Cannot set null robot agent");
        }
        
        strategy = agent;
        
        // Ensure the strategy knows its color
        if (strategy) {
            strategy->setRobotColor(robotColor);
        }
    }

    /**
     * Returns a pointer to the IRobotAgent currently assigned to this robot 
     */
    IRobotAgent* Robot::getRobotAgent() {
        return strategy;
    }

    /**
     * Updates the robot's color (red or blue) and propogates the change to the 
     * assigned strategy 
     */
    void Robot::setColor(RobotMoveRequest::RobotColor color) {
        robotColor = color;
        
        // Propagate color to strategy if exists
        if (strategy) {
            strategy->setRobotColor(color);
        }
    }

    /**
     * Returns the robot's color (RED or BLUE)
     */
    RobotMoveRequest::RobotColor Robot::getRobotColor() const {
        return robotColor;
    }

    /**
     * Destructor
     */
    Robot::~Robot() {
        strategy = nullptr;
    }
}