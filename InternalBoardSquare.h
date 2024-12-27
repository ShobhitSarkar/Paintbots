#ifndef INTERNAL_BOARD_SQUARE_H 
#define INTERNAL_BOARD_SQUARE_H 

#include "BoardSquare.h"
#include <stdexcept>
#include <cstring>  

#include <stdexcept>  
#include <algorithm> 

namespace Paintbots{
    class InternalBoardSquare : public BoardSquare {
        public: 
            InternalBoardSquare() : BoardSquare() {}

            /**
             * Setting square color with the given color 
             */
            void setSquareColor (SquareColor color); 

            /**
             * Setting a square to have red robot 
             */
            void setRedRobot(bool present); 

            /**
             * Setting a square to have blue robot 
             */
            void setBlueRobot(bool present); 

            /**
             * Setting the direction of the robot correctly 
             */
            void setRobotDirection(Direction dir); 

            /**
             * Setting the square type correctly 
             */
            void setType(SquareType sqType); 
    }; 
}

#endif 