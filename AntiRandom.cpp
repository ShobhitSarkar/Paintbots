#include "AntiRandom.h"
#include <cstdlib>
#include <cstring>  
#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {

/**
 * Constructor 
 */
AntiRandom::AntiRandom() : 
    robotColor(RobotMoveRequest::RED), 
    rng(std::time(nullptr)),
    paintBlobsLeft(30),
    moveCount(0),
    currentDirection(NORTH) {}

/**
 * Returns name of the robot 
 */
std::string AntiRandom::getRobotName() {
    return "BeatRandom";
}

/**
 * Gets robot creator 
 */
std::string AntiRandom::getRobotCreator() {
    return "Shobhit";
}

/**
 * Function to help check whether enemy is in range 
 */
bool AntiRandom::isEnemyInRange(ExternalBoardSquare** srs, Direction& enemyDir, int& distance) {
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            if((robotColor == RobotMoveRequest::RED && srs[i][j].blueRobotPresent()) ||
               (robotColor == RobotMoveRequest::BLUE && srs[i][j].redRobotPresent())) {
                distance = std::abs(i-2) + std::abs(j-2);
                if(i < 2) enemyDir = NORTH;
                else if(i > 2) enemyDir = SOUTH;
                else if(j < 2) enemyDir = WEST;
                else enemyDir = EAST;
                return true;
            }
        }
    }
    return false;
}

/**
 * Function to check if a move that is going to be be legal 
 */
bool AntiRandom::isSafeMove(ExternalBoardSquare** srs, RobotMoveRequest::RobotMove move) {
    if(move == RobotMoveRequest::FORWARD) {
        // Check if forward move is safe
        return srs[1][2].getSquareType() != WALL && 
               srs[1][2].getSquareType() != ROCK &&
               !srs[1][2].redRobotPresent() &&
               !srs[1][2].blueRobotPresent();
    }
    return true;
}

/**
 * Analyzes the current state of the board
 */
void AntiRandom::analyzeBoardState(ExternalBoardSquare** lrs, int& myTerritory, int& enemyTerritory) {
    myTerritory = 0;
    enemyTerritory = 0;
    
    for(int i = 1; i <= GameBoard::BOARD_SIZE; i++) {
        for(int j = 1; j <= GameBoard::BOARD_SIZE; j++) {
            if((robotColor == RobotMoveRequest::RED && lrs[i][j].getSquareColor() == RED) ||
               (robotColor == RobotMoveRequest::BLUE && lrs[i][j].getSquareColor() == BLUE)) {
                myTerritory++;
            } else if((robotColor == RobotMoveRequest::RED && lrs[i][j].getSquareColor() == BLUE) ||
                      (robotColor == RobotMoveRequest::BLUE && lrs[i][j].getSquareColor() == RED)) {
                enemyTerritory++;
            }
        }
    }
}

/**
 * Finds the best directions based on the position of the robot 
 */
Direction AntiRandom::findBestDirection(ExternalBoardSquare** srs) {
    int maxWhite = 0;
    Direction bestDir = currentDirection;
    
    // Check each direction for number of white squares
    for(Direction dir : {NORTH, EAST, SOUTH, WEST}) {
        int whiteCount = 0;
        if(dir == NORTH && srs[1][2].getSquareType() == EMPTY) whiteCount++;
        if(dir == EAST && srs[2][3].getSquareType() == EMPTY) whiteCount++;
        if(dir == SOUTH && srs[3][2].getSquareType() == EMPTY) whiteCount++;
        if(dir == WEST && srs[2][1].getSquareType() == EMPTY) whiteCount++;
        
        if(whiteCount > maxWhite) {
            maxWhite = whiteCount;
            bestDir = dir;
        }
    }
    
    return bestDir;
}

/**
 * Function to help decide whether a robot should shoot 
 */
bool AntiRandom::shouldShoot(Direction enemyDir) {
    if(paintBlobsLeft <= 0) return false;
    
    if(currentDirection == enemyDir) {
        return std::uniform_int_distribution<>(1,10)(rng) <= 9; // 90% chance
    }
    
    return std::uniform_int_distribution<>(1,5)(rng) == 1; // 20% chance
}

/**
 * Sets robot shooting color according to the parameter supplied 
 */
void AntiRandom::setRobotColor(RobotMoveRequest::RobotColor c) {
    robotColor = c;
}


/**
 * Gets the move after deciding on different moves 
 */
RobotMoveRequest* AntiRandom::getMove(ExternalBoardSquare** srs, ExternalBoardSquare** lrs) {
    moveCount++;
    RobotMoveRequest* request = new RobotMoveRequest();
    request->robot = robotColor;
    request->shoot = false;
    
    // Initialize shot decision
    Direction enemyDir;
    int enemyDist;
    bool enemyVisible = isEnemyInRange(srs, enemyDir, enemyDist);
    
    // Get board state information
    int myTerritory, enemyTerritory;
    analyzeBoardState(lrs, myTerritory, enemyTerritory);
    bool losing = enemyTerritory >= myTerritory;
    
    // Determine best move direction
    Direction targetDir = findBestDirection(srs);
    
    // If losing and enemy visible, prioritize attack
    if(losing && enemyVisible) {
        targetDir = enemyDir;
    }
    
    // Rotate to face target direction if needed
    if(currentDirection != targetDir) {
        // Choose shortest rotation
        int clockwise = 0, counterClockwise = 0;
        Direction tempDir = currentDirection;
        
        // Count clockwise rotations needed
        while(tempDir != targetDir) {
            clockwise++;
            switch(tempDir) {
                case NORTH: tempDir = EAST; break;
                case EAST: tempDir = SOUTH; break;
                case SOUTH: tempDir = WEST; break;
                case WEST: tempDir = NORTH; break;
            }
        }
        
        // Count counterclockwise rotations needed
        tempDir = currentDirection;
        while(tempDir != targetDir) {
            counterClockwise++;
            switch(tempDir) {
                case NORTH: tempDir = WEST; break;
                case WEST: tempDir = SOUTH; break;
                case SOUTH: tempDir = EAST; break;
                case EAST: tempDir = NORTH; break;
            }
        }
        
        // Choose shorter rotation
        if(clockwise <= counterClockwise) {
            request->move = RobotMoveRequest::ROTATERIGHT;
            switch(currentDirection) {
                case NORTH: currentDirection = EAST; break;
                case EAST: currentDirection = SOUTH; break;
                case SOUTH: currentDirection = WEST; break;
                case WEST: currentDirection = NORTH; break;
            }
        } else {
            request->move = RobotMoveRequest::ROTATELEFT;
            switch(currentDirection) {
                case NORTH: currentDirection = WEST; break;
                case WEST: currentDirection = SOUTH; break;
                case SOUTH: currentDirection = EAST; break;
                case EAST: currentDirection = NORTH; break;
            }
        }
    }
    // Move forward if facing target direction and safe
    else if(isSafeMove(srs, RobotMoveRequest::FORWARD)) {
        request->move = RobotMoveRequest::FORWARD;
    }


    // Rotate if can't move forward
    else {
        request->move = RobotMoveRequest::ROTATERIGHT;
        switch(currentDirection) {
            case NORTH: currentDirection = EAST; break;
            case EAST: currentDirection = SOUTH; break;
            case SOUTH: currentDirection = WEST; break;
            case WEST: currentDirection = NORTH; break;
        }
    }
    
    // Decide whether to shoot
    if(enemyVisible) {
        request->shoot = shouldShoot(enemyDir);
        if(request->shoot) paintBlobsLeft--;
    }
    
    return request;
}

}