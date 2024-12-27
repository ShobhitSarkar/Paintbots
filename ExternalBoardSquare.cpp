#include "ExternalBoardSquare.h"
#include <iostream>
#include <cstring>  
#include <stdexcept>  
#include <algorithm> 

namespace Paintbots{

    /**
     * Constructor based on BoardSquare constructor 
     */
ExternalBoardSquare::ExternalBoardSquare(const InternalBoardSquare& square):
    BoardSquare(square.getSquareColor(), 
                square.redRobotPresent(), 
                square.blueRobotPresent(), 
                square.robotDirection(), 
                square.getSquareType()) {}

}