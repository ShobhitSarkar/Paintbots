#ifndef GAME_BOARD_H
#define GAME_BOARD_H
#include "InternalBoardSquare.h"
#include "ExternalBoardSquare.h"
#include "Config.h"
#include "ObserverPattern.h"
#include <string>
#include <cstring>  
#include <stdexcept>  
#include <algorithm> 

namespace Paintbots{
/**
 * @struct Bundles all the necessary information about a robot move 
 */
struct RobotMoveRequest {

    enum RobotColor{
        RED, 
        BLUE
    }; 

    enum RobotMove{
        ROTATELEFT, 
        ROTATERIGHT, 
        FORWARD, 
        NONE
    }; 

    RobotColor robot; 
    RobotMove move; 
    bool shoot; 
}; 

class GameBoard : public Observable {
    
    public: 

        static const int BOARD_SIZE = 15; // constant to hold the board size 
        static GameBoard* getInstance(const std::string& password); // constructor to allow singleton access 
        ~GameBoard(); // destructor 

        /**
         * Gets the square at a particular row and col 
         */
        InternalBoardSquare& getSquare(int row, int col); 

        /**
         * Method to move the robot 
         */
        bool moveRobot(RobotMoveRequest& mr); 

        /**
         * Method to handle the case when a robot gets hit with a paint blob 
         */
        bool paintBlobHit(RobotMoveRequest& mr); 

        /**
         * Method to return the blue robot's score 
         */
        int blueScore(); 

        /**
         * Method to return the red robot's score 
         */
        int redScore(); 

        /**
         * Method to set the color of a particular square 
         */
        void setSquareColor(int row, int col, SquareColor color); 

        /**
         * Method to set a robot to paint the other color 
         */
        void setRobotPaintColor(RobotMoveRequest::RobotColor robot, SquareColor color); 

        /**
         * Method to get the full view of the board 
         */
        ExternalBoardSquare** getLongRangeScan(); 

        /**
         * Method to get a 5*5 view with the robot in the center with the direction of 
         * the robot being considered up 
         */
        ExternalBoardSquare** getShortRangeScan(RobotMoveRequest::RobotColor rc); 

        /**
         * Get remaining paint blobs for a robot
         */
        int getRemainingPaintBlobs(RobotMoveRequest::RobotColor robot) const;

        /**
         * Cleaning up memory 
         */
        static void resetInstance() {
            delete instance;
            instance = nullptr;
        }
    
    private: 
        static GameBoard* instance; // instance for gameboard 
        static const std::string PASSWORD; // string for password 
        SquareColor redRobotPaintColor; // keeping track of what color the red robot is currently shooting 
        SquareColor blueRobotPaintColor; // keeping track of what color the blue robot is currently shooting 
        InternalBoardSquare board[BOARD_SIZE  + 2][BOARD_SIZE + 2]; // InternalBoardSquare of correct size 
        Config& config; // Config object for keeping track of config values 
        int redPaintBlobsRemaining;    // Track remaining paint blobs for red robot
        int bluePaintBlobsRemaining;   // Track remaining paint blobs for blue robot

        //Private constructors for singleton implementation 
        GameBoard(); // default constructor 
        GameBoard(const Config& config); // specific constructor 
        GameBoard(const GameBoard&) = delete; // prevent copying 
        GameBoard& operator = (const GameBoard&) = delete; // prevent assignment 

        int redRobotHitDuration; // var to keep track of remaining moves when it gets hit 
        int blueRobotHitDuration; // var to keep track of remaining moves when blue gets hit 

        /**
         * Validates whether a position is valid 
         */
        bool isValidPosition(int row, int col) const; 

        /**
         * Initializes board 
         */
        void initializeBoard(); 

        /**
         * Method to place walls on the board 
         */
        void placeWalls(); 

        /**
         * Method to place rocks on the board 
         */
        void placeRocks(); 

        /**
         * Method to place fog on the board 
         */
        void placeFog(); 

        /**
         * Method to place robots on the board 
         */
        void placeRobots(); 

};
}
#endif