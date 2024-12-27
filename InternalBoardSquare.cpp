#include "InternalBoardSquare.h"

#include <cstring>  

#include <stdexcept>  
#include <algorithm> 

namespace Paintbots{
    /**
     * Setting square color 
     */
    void InternalBoardSquare::setSquareColor(SquareColor color) {
        if (color != RED && color != BLUE && color != WHITE) {
            throw std::runtime_error("Invalid color!");
        }
        squareColor = color;
    }

    /**
     * setting a square to have the red robot 
     */
    void InternalBoardSquare::setRedRobot(bool present){
        if (present && hasBlueRobot){
            throw std::runtime_error("cannot place red robot: square occupied by blue robot"); 
        }

        hasRedRobot = present; 
    }

    /**
     * setting a square to have the blue robot 
     */
    void InternalBoardSquare::setBlueRobot(bool present){
        if (present && hasRedRobot){
            throw std::runtime_error("cannot place blue robot: square occupied by red robot"); 
        }
        hasBlueRobot = present; 
    }

    /**
     * setting correct direction of the robot 
     */
    void InternalBoardSquare::setRobotDirection(Direction dir){
        robotFacing = dir; 
    }

    /**
     * setting the correct square type 
     */
    void InternalBoardSquare::setType(SquareType newType){
        if (squareType == ROCK && newType == FOG){
            throw std::runtime_error("Can't place fog on rock"); 
        }
        if (squareType == FOG && newType == ROCK){
            throw std::runtime_error("Can't place rock on fog"); 
        }
        squareType = newType; 
    }
}