#include "RandomRobot.h"

#include <cstring>  

#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {

    /**
     * Default constructor for RandomRobot
     * 
     * Initializes the robot's attributes such as color, random number generator, etc..
     */
    RandomRobot::RandomRobot() : 
        robotColor(RobotMoveRequest::RED), 
        rng(std::time(nullptr)),
        paintBlobsLeft(30),
        lastMove(RobotMoveRequest::NONE) {}

    /**
     * Returns string representing the robot's name 
     */
    std::string RandomRobot::getRobotName() {
        return "RandomRobot";
    }

    /**
     * Returns string representing the creator's name 
     */
    std::string RandomRobot::getRobotCreator() {
        return "Shobhit";
    }

    /**
     * Chooses a random movement direction based on the short range scan (SRS)
     */
    RobotMoveRequest::RobotMove RandomRobot::getRandomMove(ExternalBoardSquare** srs) {
        std::uniform_int_distribution<> moveDist(0, 2);
        int moveIndex = moveDist(rng);
        
        switch(moveIndex) {
            case 0: return RobotMoveRequest::ROTATELEFT;
            case 1: return RobotMoveRequest::ROTATERIGHT;
            case 2: 
                if (srs[1][2].getSquareType() == EMPTY &&
                    !srs[1][2].redRobotPresent() &&
                    !srs[1][2].blueRobotPresent()) {
                    return RobotMoveRequest::FORWARD;
                }
                return RobotMoveRequest::ROTATELEFT;
            default: 
                return RobotMoveRequest::NONE;
        }
    }

    /**
     * Decides the robot's next move based on the game board state =
     * 
     * Primarily behvaior includes: 
     * - Aggressive mode: tries to face and attack enemy if loosing territory 
     * - Exploratory mode: moves and paints territory randomly when no enemies are in range 
     * - Shooting: Fires paint if the enemy is in range absed on random choices 
     */
    RobotMoveRequest* RandomRobot::getMove(ExternalBoardSquare** srs, ExternalBoardSquare** lrs) {
        RobotMoveRequest* request = new RobotMoveRequest();
        request->robot = robotColor;
        
        // Get current state
        Direction currentDir = srs[2][2].robotDirection();
        
        // Analyze long range scan for strategy
        int myTerritory = 0, enemyTerritory = 0;
        for(int i = 0; i < GameBoard::BOARD_SIZE + 2; i++) {
            for(int j = 0; j < GameBoard::BOARD_SIZE + 2; j++) {
                if((robotColor == RobotMoveRequest::RED && lrs[i][j].getSquareColor() == RED) ||
                (robotColor == RobotMoveRequest::BLUE && lrs[i][j].getSquareColor() == BLUE)) {
                    myTerritory++;
                } else if((robotColor == RobotMoveRequest::RED && lrs[i][j].getSquareColor() == BLUE) ||
                        (robotColor == RobotMoveRequest::BLUE && lrs[i][j].getSquareColor() == RED)) {
                    enemyTerritory++;
                }
            }
        }
        
        // If losing, be more aggressive
        bool aggressive = enemyTerritory > myTerritory;
        
        // Look for enemy in short range
        bool enemyInRange = false;
        Direction enemyDir = NORTH;
        int enemyDist = 999;
        
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < 5; j++) {
                if((robotColor == RobotMoveRequest::RED && srs[i][j].blueRobotPresent()) ||
                (robotColor == RobotMoveRequest::BLUE && srs[i][j].redRobotPresent())) {
                    enemyInRange = true;
                    int dist = abs(i-2) + abs(j-2);
                    if(dist < enemyDist) {
                        enemyDist = dist;
                        if(i < 2) enemyDir = NORTH;
                        else if(i > 2) enemyDir = SOUTH;
                        else if(j < 2) enemyDir = WEST;
                        else enemyDir = EAST;
                    }
                }
            }
        }

        // Decide movement
        if(enemyInRange && aggressive) {
            // Try to face enemy for shooting
            if(currentDir != enemyDir) {
                request->move = (moveCount % 2 == 0) ? 
                    RobotMoveRequest::ROTATELEFT : 
                    RobotMoveRequest::ROTATERIGHT;
            } else if(srs[1][2].getSquareType() != WALL && 
                    srs[1][2].getSquareType() != ROCK) {
                request->move = RobotMoveRequest::FORWARD;
            }
        } else {
            // Explore and paint territory
            std::uniform_int_distribution<> moveDist(0, 9);
            int moveChoice = moveDist(rng);
            
            if(moveChoice < 5 && srs[1][2].getSquareType() != WALL && 
            srs[1][2].getSquareType() != ROCK) {
                request->move = RobotMoveRequest::FORWARD;
            } else if(moveChoice < 7) {
                request->move = RobotMoveRequest::ROTATELEFT;
            } else if(moveChoice < 9) {
                request->move = RobotMoveRequest::ROTATERIGHT;
            } else {
                request->move = RobotMoveRequest::NONE;
            }
        }

        // Shoot if we have paint and either:
        // 1. Enemy in range and facing them
        // 2. Random chance when exploring
        request->shoot = paintBlobsLeft > 0 && 
                        ((enemyInRange && currentDir == enemyDir) ||
                        (!enemyInRange && std::uniform_int_distribution<>(1,5)(rng) == 1));
                        
        if(request->shoot) paintBlobsLeft--;

        return request;
    }

    /**
     * Sets the robot to paint a certain color 
     */
    void RandomRobot::setRobotColor(RobotMoveRequest::RobotColor c) {
        robotColor = c;
    }
}