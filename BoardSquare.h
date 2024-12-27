#ifndef BOARD_SQUARE_H
#define BOARD_SQUARE_H
#include <cstring>  
#include <stdexcept>  
#include <algorithm> 

namespace Paintbots{

    /**
     * @enum Keeping track of the possible directions robot could move in 
     */
    enum Direction{
        NORTH, 
        SOUTH, 
        EAST,
        WEST
    }; 

    /**
     * @enum Enum to keep track of different square types 
     */
    enum SquareType {
        EMPTY, 
        ROCK, 
        FOG, 
        WALL
    }; 

    /**
     * @enum Enum to keep track of the colors of squares 
     */
    enum SquareColor{
        RED = 0, 
        BLUE = 1, 
        WHITE = 2
    }; 

    /**
     * @class Base class for InternalBoardSquare and ExternalBoardSquare
     */
    class BoardSquare{
        protected: 
            SquareColor squareColor; // keep track of square color 
            bool hasRedRobot; // keep track if a square has a red robot 
            bool hasBlueRobot; // keep track if a square has a blue robot 
            Direction robotFacing; // keep track of the direction the robot is facing 
            SquareType squareType; // keep track of the different square types 

        public: 
            // default constructor
            BoardSquare(): 
                squareColor(WHITE), 
                hasRedRobot(false), 
                hasBlueRobot(false), 
                robotFacing(NORTH), 
                squareType(EMPTY) {}


            // specific constructor 
            BoardSquare(SquareColor color, bool redRobot, bool blueRobot, 
                   Direction direction, SquareType type) :
                squareColor(color),
                hasRedRobot(redRobot),
                hasBlueRobot(blueRobot),
                robotFacing(direction),
                squareType(type) {}

            // Copy Constructor 
            BoardSquare(const BoardSquare& other) : 
                squareColor(other.squareColor), 
                hasRedRobot(other.hasRedRobot), 
                hasBlueRobot(other.hasBlueRobot), 
                robotFacing(other.robotFacing), 
                squareType(other.squareType) {}

            
            // assignment operator
            BoardSquare& operator=(const BoardSquare& other){
                if (this != &other){
                    squareColor = other.squareColor; 
                    hasRedRobot = other.hasRedRobot; 
                    hasBlueRobot = other.hasBlueRobot; 
                    robotFacing = other.robotFacing; 
                    squareType = other.squareType; 
                }
                return *this; 
            }

            // virtual deconstructor 
            virtual ~BoardSquare(){}

            //inline getters 

            /**
             * Gets color of the square 
             */
            inline SquareColor getSquareColor() const {return squareColor;}

            /**
             * Gets type of the square 
             */
            inline SquareType getSquareType() const {return squareType;}

            /**
             * @return true if red robot is present in a square 
             */
            inline bool redRobotPresent() const {return hasRedRobot;}

            /**
             * @return true if blue robot is present in a square 
             */
            inline bool blueRobotPresent() const {return hasBlueRobot;}

            /**
             * Returns the direction the robot is facing towards
             */
            inline Direction robotDirection() const {return robotFacing;}
    };
}

#endif