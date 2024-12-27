#include "LazyRobot.h"
#include <random>

#include <cstring>  

#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {

    /**
     * Constructs a LazyRobot instance 
     */
    LazyRobot::LazyRobot() : 
        robotColor(RobotMoveRequest::RED), 
        rng(std::time(nullptr)),
        moveCount(0),
        paintBlobsLeft(30),
        lastDirection(NORTH) {}

    /**
     * Gets the name of the robot 
     * @return a string "LazyRobot"
     */
    std::string LazyRobot::getRobotName() {
        return "LazyRobot";
    }

    /**
     * Returns the name of the robot's creator 
     */
    std::string LazyRobot::getRobotCreator() {
        return "Shobhit";
    }

    /**
     * Determines the next move for the robot. 
     * 
     * Analyzes the board using the short and long range scans to decide robot's movement and shooting actions 
     */
    RobotMoveRequest* LazyRobot::getMove(ExternalBoardSquare** srs, ExternalBoardSquare** lrs) {
        moveCount++;
        RobotMoveRequest* request = new RobotMoveRequest();
        request->robot = robotColor;
        
        // Check surrounding squares for enemy robot
        bool enemyNearby = false;
        Direction enemyDirection = NORTH;
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < 5; j++) {
                if((robotColor == RobotMoveRequest::RED && srs[i][j].blueRobotPresent()) ||
                (robotColor == RobotMoveRequest::BLUE && srs[i][j].redRobotPresent())) {
                    enemyNearby = true;
                    if(i < 2) enemyDirection = NORTH;
                    else if(i > 2) enemyDirection = SOUTH;
                    else if(j < 2) enemyDirection = WEST;
                    else enemyDirection = EAST;
                }
            }
        }

        // Move if:
        // 1. Surrounded by opponent's color
        // 2. Near a wall
        // 3. In fog
        // 4. Every 5 moves to avoid being too static
        bool needsToMove = false;
        if(srs[2][2].getSquareType() == FOG || moveCount % 5 == 0) {
            needsToMove = true;
        }

        // Count opponent's color around us
        int opponentColors = 0;
        for(int i = 1; i < 4; i++) {
            for(int j = 1; j < 4; j++) {
                if((robotColor == RobotMoveRequest::RED && srs[i][j].getSquareColor() == BLUE) ||
                (robotColor == RobotMoveRequest::BLUE && srs[i][j].getSquareColor() == RED)) {
                    opponentColors++;
                }
            }
        }
        
        if(opponentColors > 2) needsToMove = true;

        if(needsToMove) {
            // Move forward if safe, otherwise rotate
            if(srs[1][2].getSquareType() != WALL && 
            srs[1][2].getSquareType() != ROCK &&
            !srs[1][2].redRobotPresent() &&
            !srs[1][2].blueRobotPresent()) {
                request->move = RobotMoveRequest::FORWARD;
            } else {
                request->move = (moveCount % 2 == 0) ? 
                    RobotMoveRequest::ROTATELEFT : 
                    RobotMoveRequest::ROTATERIGHT;
            }
        } else {
            request->move = RobotMoveRequest::NONE;
        }

        // Only shoot if we have paintblobs and either:
        // 1. Enemy is nearby
        // 2. Random chance (1 in 3) to try to hit moving enemies
        request->shoot = paintBlobsLeft > 0 && 
                        (enemyNearby || (std::uniform_int_distribution<>(1,3)(rng) == 1));
        if(request->shoot) paintBlobsLeft--;

        return request;
    }

    void LazyRobot::setRobotColor(RobotMoveRequest::RobotColor c) {
        robotColor = c;
    }
} 