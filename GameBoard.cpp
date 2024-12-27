#include "GameBoard.h"
#include <stdexcept> 
#include <random> 
#include <ctime> 
#include <iostream>
#include <cstring> 
#include <stdexcept>  
#include <algorithm>  

namespace Paintbots{

    GameBoard* GameBoard::instance = nullptr; 
    const std::string GameBoard::PASSWORD = "xyzzy"; 

    /**
     * Default constructor 
     */
    GameBoard::GameBoard() : 
        redRobotPaintColor(RED),
        blueRobotPaintColor(BLUE),
        config(Config::getInstance()),
        redPaintBlobsRemaining(config.getPaintBlobLimit()),
        bluePaintBlobsRemaining(config.getPaintBlobLimit()),
        redRobotHitDuration(0),
        blueRobotHitDuration(0) {
        initializeBoard();
    }


    /**
     * Takes a reference to a Config object and initializes a board 
     */
    GameBoard::GameBoard(const Config& config) : 
        redRobotPaintColor(RED),
        blueRobotPaintColor(BLUE),
        config(const_cast<Config&>(config)),
        redPaintBlobsRemaining(config.getPaintBlobLimit()),
        bluePaintBlobsRemaining(config.getPaintBlobLimit()),
        redRobotHitDuration(0),
        blueRobotHitDuration(0) {
        initializeBoard();
    }

    /**
     * Gets GameBoard instance by passing in the password 
     */
    GameBoard* GameBoard::getInstance(const std::string& password){
        if (password.empty() || password != PASSWORD){
            throw std::runtime_error("Invalid access attempt to GameBoard");
        }

        if (instance == nullptr){
            instance = new GameBoard(); 
        }

        return instance; 
    }


    /**
     * Cleaning up GameBoard 
     */
    GameBoard::~GameBoard() {
        if (instance == this) {
            instance = nullptr;
        }
        
        for (Observer* observer : getObservers()) {
            removeObserver(observer);
        }
    }

    /**
     * Places walls, rocks, fog and robots on the board 
     */
    void GameBoard::initializeBoard(){
        placeWalls(); 
        placeRocks(); 
        placeFog(); 
        placeRobots(); 
    }

    /**
     * Function to place walls on the board 
     */
    void GameBoard::placeWalls() {
        for (int i = 0; i < BOARD_SIZE + 2; ++i) {
            board[0][i].setType(WALL);
            board[BOARD_SIZE + 1][i].setType(WALL);
            board[i][0].setType(WALL);
            board[i][BOARD_SIZE + 1].setType(WALL);
        }
    }

    /**
     * Function to place rocks on the board 
     */
    void GameBoard::placeRocks(){
        std::random_device rd; 
        std::mt19937 gen(rd()); 
        std::uniform_int_distribution<> rowDist(1, BOARD_SIZE); 
        std::uniform_int_distribution<>colDist(1, BOARD_SIZE); 

        int numRocks = std::uniform_int_distribution<>(
            config.getRockLowerBound(), 
            config.getRockUpperBound()
        )(gen); 

        for (int i = 0; i < numRocks; ++i){
            int row = rowDist(gen); 
            int col = colDist(gen); 
            if (board[row][col].getSquareType() == EMPTY){
                board[row][col].setType(ROCK); 
            } else {
                --i; 
            }
        }
    }

    /**
     * Function to place fog on the board 
     */
    void GameBoard::placeFog(){
        std::random_device rd; 
        std::mt19937 gen(rd()); 
        std::uniform_int_distribution<> rowDist(1, BOARD_SIZE); 
        std::uniform_int_distribution<> colDist(1, BOARD_SIZE); 

        int numFog = std::uniform_int_distribution<> (  
            config.getFogLowerBound(), 
            config.getFogUpperBound()
        )(gen);


        for (int i = 0; i < numFog; ++i){
            int row = rowDist(gen); 
            int col = colDist(gen); 
            if (board[row][col].getSquareType() == EMPTY){
                board[row][col].setType(FOG); 
            } else {
                --i; 
            }
        }
    }

    /**
     * Function to place robots on the board 
     */
    void GameBoard::placeRobots() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> rowDist(1, BOARD_SIZE);
        std::uniform_int_distribution<> colDist(1, BOARD_SIZE);

        // Placing red robot
        int redRobotPlaced = 0;
        while (redRobotPlaced == 0) {
            int row = rowDist(gen);
            int col = colDist(gen);
            if (board[row][col].getSquareType() == EMPTY && 
                !board[row][col].redRobotPresent() && 
                !board[row][col].blueRobotPresent()) {
                
                board[row][col].setRedRobot(true);
                board[row][col].setRobotDirection(NORTH);  // Default direction
                board[row][col].setSquareColor(WHITE);     // Ensure initial square color is white
                
                std::cout << "Red robot placed at (" << row << ", " << col << ")" << std::endl;
                redRobotPlaced = 1;
            }
        }

        // Placing blue robot
        int blueRobotPlaced = 0;
        while (blueRobotPlaced == 0) {
            int row = rowDist(gen);
            int col = colDist(gen);
            if (board[row][col].getSquareType() == EMPTY && 
                !board[row][col].redRobotPresent() && 
                !board[row][col].blueRobotPresent()) {
                
                board[row][col].setBlueRobot(true);
                board[row][col].setRobotDirection(NORTH);  // Default direction
                board[row][col].setSquareColor(WHITE);     // Ensure initial square color is white
                
                std::cout << "Blue robot placed at (" << row << ", " << col << ")" << std::endl;
                blueRobotPlaced = 1;
            }
        }

        // Additional debug logging to confirm robot placement
        for (int i = 1; i <= BOARD_SIZE; ++i) {
            for (int j = 1; j <= BOARD_SIZE; ++j) {
                if (board[i][j].redRobotPresent()) {
                    std::cout << "Verification: Red robot at (" << i << ", " << j << ")" << std::endl;
                }
                if (board[i][j].blueRobotPresent()) {
                    std::cout << "Verification: Blue robot at (" << i << ", " << j << ")" << std::endl;
                }
            }
        }
    }

    /**
     * Method to check is a postion is valid for placing a robot 
     */
    bool GameBoard::isValidPosition(int row, int col) const {
        return row>=0 && row < BOARD_SIZE + 2 && 
                col >= 0 && col < BOARD_SIZE + 2; 
    }

    /**
     * Method that returns a square 
     */
    InternalBoardSquare& GameBoard::getSquare(int row, int col){
        if (!isValidPosition(row, col)){
            throw std::out_of_range("Invalid board position"); 
        } 

        return board[row][col]; 
    }

    /**
     * Performs a the requested move on the designated robot  
     */
    bool GameBoard::moveRobot(RobotMoveRequest& mr) {
        // Find current robot position and direction
        int robotRow = -1, robotCol = -1;
        Direction robotDir;
        
        // Find the robot's position and direction
        for (int i = 1; i <= BOARD_SIZE; ++i) {
            for (int j = 1; j <= BOARD_SIZE; ++j) {
                if ((mr.robot == RobotMoveRequest::RED && board[i][j].redRobotPresent()) || 
                    (mr.robot == RobotMoveRequest::BLUE && board[i][j].blueRobotPresent())) {
                    robotRow = i;
                    robotCol = j;
                    robotDir = board[i][j].robotDirection();
                    break;
                }
            }
            if (robotRow != -1) break;
        }

        // If robot not found, throw appropriate error
        if (robotRow == -1) {
            throw std::runtime_error("Robot not found on board");
        }

        InternalBoardSquare& currentSquare = board[robotRow][robotCol];

        // Update hit durations before move
        if (mr.robot == RobotMoveRequest::RED) {
            if (redRobotHitDuration > 0) {
                redRobotHitDuration--;
                if (redRobotHitDuration == 0) {
                    redRobotPaintColor = RED; 
                    notifyObservers();
                }
            }
        } else {  // BLUE robot
            if (blueRobotHitDuration > 0) {
                blueRobotHitDuration--;
                if (blueRobotHitDuration == 0) {
                    blueRobotPaintColor = BLUE; 
                    notifyObservers();
                }
            }
        }

        // Handle different move types
        switch(mr.move) {
            case RobotMoveRequest::FORWARD: {
                // Calculate next position based on direction
                int nextRow = robotRow;
                int nextCol = robotCol;
                
                switch(robotDir) {
                    case NORTH: nextRow--; break;
                    case SOUTH: nextRow++; break;
                    case EAST: nextCol++; break;
                    case WEST: nextCol--; break;
                    default:
                        throw std::invalid_argument("Invalid robot direction");
                }

                // Check if next square is valid
                if (!isValidPosition(nextRow, nextCol)) {
                    return false;
                }

                InternalBoardSquare& nextSquare = board[nextRow][nextCol];

                // Check for obstacles
                if (nextSquare.getSquareType() == WALL || 
                    nextSquare.getSquareType() == ROCK ||
                    nextSquare.redRobotPresent() || 
                    nextSquare.blueRobotPresent()) {
                    return false;
                }

                // Determine paint color based on robot and hit status
                SquareColor paintColor;
                if (mr.robot == RobotMoveRequest::RED) {
                    paintColor = redRobotPaintColor;
                } else {
                    paintColor = blueRobotPaintColor;
                }

                // Paint both the square we're leaving and the square we're moving to
                currentSquare.setSquareColor(paintColor);
                nextSquare.setSquareColor(paintColor);
                
                // Move robot to new square
                if (mr.robot == RobotMoveRequest::RED) {
                    currentSquare.setRedRobot(false);
                    nextSquare.setRedRobot(true);
                } else {
                    currentSquare.setBlueRobot(false);
                    nextSquare.setBlueRobot(true);
                }
                
                nextSquare.setRobotDirection(robotDir);
                notifyObservers();
                break;
            }
            
            case RobotMoveRequest::ROTATELEFT: {
                Direction newDir;
                switch(robotDir) {
                    case NORTH: newDir = WEST; break;
                    case WEST: newDir = SOUTH; break;
                    case SOUTH: newDir = EAST; break;
                    case EAST: newDir = NORTH; break;
                    default:
                        throw std::invalid_argument("Invalid robot direction");
                }
                currentSquare.setRobotDirection(newDir);
                notifyObservers();
                break;
            }
            
            case RobotMoveRequest::ROTATERIGHT: {
                Direction newDir;
                switch(robotDir) {
                    case NORTH: newDir = EAST; break;
                    case EAST: newDir = SOUTH; break;
                    case SOUTH: newDir = WEST; break;
                    case WEST: newDir = NORTH; break;
                    default:
                        throw std::invalid_argument("Invalid robot direction");
                }
                currentSquare.setRobotDirection(newDir);
                notifyObservers();
                break;
            }
            
            case RobotMoveRequest::NONE:
                notifyObservers();
                break;
            
            default:
                throw std::invalid_argument("Invalid move type");
        }

        return true;
    }

    /**
     * Determines if a paint blob shot by a robot hits the opponent 
     * 
     * Traces the trajectory of a paint blob from the shooting robot's position in the direction 
     * the robot is facing. Also handles: 
     * - Paint blob limit tracking 
     * - Blocking by walls or rocks 
     * - Hitting opponent robots 
     * - Updating hit duration and paint color 
     */
    bool GameBoard::paintBlobHit(RobotMoveRequest& mr) {
        if (!mr.shoot) return false;
        
        // Check paint blob limit
        if (mr.robot == RobotMoveRequest::RED) {
            if (redPaintBlobsRemaining <= 0) {
                return false;
            }
        } else {
            if (bluePaintBlobsRemaining <= 0) {
                return false;
            }
        }

        // Find current robot position and direction
        int shooterRow = -1, shooterCol = -1;
        Direction shooterDir;

        if (mr.robot != RobotMoveRequest::RED && mr.robot != RobotMoveRequest::BLUE) {
            throw std::invalid_argument("Invalid robot color for paint blob");
        }

        // Find shooter's position and direction
        for (int i = 1; i <= BOARD_SIZE; ++i) {
            for (int j = 1; j <= BOARD_SIZE; ++j) {
                if ((mr.robot == RobotMoveRequest::RED && board[i][j].redRobotPresent()) || 
                    (mr.robot == RobotMoveRequest::BLUE && board[i][j].blueRobotPresent())) {
                    shooterRow = i;
                    shooterCol = j;
                    shooterDir = board[i][j].robotDirection();
                    break;
                }
            }
            if (shooterRow != -1) break;
        }

        // If shooter not found, throw exception
        if (shooterRow == -1) {
            throw std::runtime_error("Robot not found on board");
        }

        // Calculate direction offsets
        int rowOffset = 0, colOffset = 0;
        switch (shooterDir) {
            case NORTH: rowOffset = -1; break;
            case SOUTH: rowOffset = 1; break;
            case EAST: colOffset = 1; break;
            case WEST: colOffset = -1; break;
            default:
                throw std::invalid_argument("Invalid robot direction");
        }

        // Check along the line of fire
        int currentRow = shooterRow + rowOffset;
        int currentCol = shooterCol + colOffset;

        while (isValidPosition(currentRow, currentCol)) {
            InternalBoardSquare& currentSquare = board[currentRow][currentCol];

            if (currentSquare.getSquareType() == WALL || 
                currentSquare.getSquareType() == ROCK) {
                return false;
            }

            // Check for hit on opponent
            bool hitDetected = false;
            if (mr.robot == RobotMoveRequest::RED && currentSquare.blueRobotPresent()) {
                blueRobotHitDuration = config.getHitDuration();
                blueRobotPaintColor = RED;
                redPaintBlobsRemaining--;  // Decrement paint blobs only on successful hit
                hitDetected = true;
            } else if (mr.robot == RobotMoveRequest::BLUE && currentSquare.redRobotPresent()) {
                redRobotHitDuration = config.getHitDuration();
                redRobotPaintColor = BLUE;
                bluePaintBlobsRemaining--;  // Decrement paint blobs only on successful hit
                hitDetected = true;
            }

            if (hitDetected) {
                notifyObservers();
                return true;
            }

            currentRow += rowOffset;
            currentCol += colOffset;
        }

        return false;
    }

    /**
     * Method to return the blue robot's score 
     */
    int GameBoard::blueScore(){
        int score = 0; 
        
        for (int i = 1; i <= BOARD_SIZE; ++i){
            for (int j = 1; j <= BOARD_SIZE; ++j){
                if (board[i][j].getSquareColor() == BLUE){
                    ++score; 
                }
            }
        }

        notifyObservers(); 
        return score; 
    }

    /**
     * Method to return the red robot's score 
     */
    int GameBoard::redScore() {
        int score = 0;
        for (int i = 1; i <= BOARD_SIZE; ++i) {
            for (int j = 1; j <= BOARD_SIZE; ++j) {
                SquareColor color = board[i][j].getSquareColor();
                if (color == RED) {
                    ++score;
                }
            }
        }
        return score;
    }

    /**
     * Method to set a square on the gameboard to a particular color 
     */
    void GameBoard::setSquareColor(int row, int col, SquareColor color) {
        if (!isValidPosition(row, col)) {
            throw std::out_of_range("Invalid board position");
        }
        board[row][col].setSquareColor(color);
        notifyObservers();  // Ensure this is being called
    }

    /**
     * Method to set a robot the other color in case it gets hit by the paintblob 
     */
    void GameBoard::setRobotPaintColor(RobotMoveRequest::RobotColor robot, SquareColor color) {
        if (color != RED && color != BLUE) {
            throw std::invalid_argument("Invalid color: must be red or blue only.\n");
        }
        
        if (robot == RobotMoveRequest::RED) {
            redRobotPaintColor = color;
        } else {
            blueRobotPaintColor = color;
        }
        notifyObservers();
    }

    /**
     * Gets a view of the entire board 
     */
    ExternalBoardSquare** GameBoard::getLongRangeScan() {
        ExternalBoardSquare** scan = new ExternalBoardSquare*[BOARD_SIZE + 2];
        for (int i = 0; i < BOARD_SIZE + 2; ++i) {
            scan[i] = new ExternalBoardSquare[BOARD_SIZE + 2]; 
            for (int j = 0; j < BOARD_SIZE + 2; ++j) {
                scan[i][j] = ExternalBoardSquare(board[i][j]);
            }
        }
        return scan;
    }   

    /**
     * Gets a 5*5 view of the board with the robot being in the center and the 
     * direction that is facing is considered up. 
     */
    ExternalBoardSquare** GameBoard::getShortRangeScan(RobotMoveRequest::RobotColor rc) {
        // Find robot position and direction
        int robotRow = -1, robotCol = -1;
        Direction robotDir;

        if (rc != RobotMoveRequest::RED && rc != RobotMoveRequest::BLUE) {
            throw std::invalid_argument("Invalid robot color for scan");
        }
        
        // Find the robot's position and direction
        for (int i = 1; i <= BOARD_SIZE; ++i) {
            for (int j = 1; j <= BOARD_SIZE; ++j) {
                if ((rc == RobotMoveRequest::RED && board[i][j].redRobotPresent()) || 
                    (rc == RobotMoveRequest::BLUE && board[i][j].blueRobotPresent())) {
                    robotRow = i;
                    robotCol = j;
                    robotDir = board[i][j].robotDirection();
                    break;
                }
            }
            if (robotRow != -1) break;
        }

        if (robotRow == -1) {
            throw std::runtime_error("Robot not found on board");
        }

        // Create 5x5 scan array
        ExternalBoardSquare** scan = new ExternalBoardSquare*[5];
        for (int i = 0; i < 5; ++i) {
            scan[i] = new ExternalBoardSquare[5];
        }

        // If robot is in fog, return all empty squares except center
        if (board[robotRow][robotCol].getSquareType() == FOG) {
            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < 5; ++j) {
                    if (i == 2 && j == 2) {
                        scan[i][j] = ExternalBoardSquare(board[robotRow][robotCol]);
                    } else {
                        scan[i][j] = ExternalBoardSquare();  // Empty square
                    }
                }
            }
            return scan;
        }

        // Calculate scan based on robot's direction
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                // Calculate position relative to robot (robot is at center [2][2])
                int relRow = i - 2;
                int relCol = j - 2;
                
                // Transform coordinates based on robot's direction
                int boardRow, boardCol;
                switch(robotDir) {
                    case NORTH:
                        boardRow = robotRow + relRow;
                        boardCol = robotCol + relCol;
                        break;
                    case EAST:
                        boardRow = robotRow + relCol;
                        boardCol = robotCol - relRow;
                        break;
                    case SOUTH:
                        boardRow = robotRow - relRow;
                        boardCol = robotCol - relCol;
                        break;
                    case WEST:
                        boardRow = robotRow - relCol;
                        boardCol = robotCol + relRow;
                        break;
                }

                // Check if position is within board bounds
                if (boardRow >= 0 && boardRow < BOARD_SIZE + 2 && 
                    boardCol >= 0 && boardCol < BOARD_SIZE + 2) {
                    scan[i][j] = ExternalBoardSquare(board[boardRow][boardCol]);
                } else {
                    // Out of bounds shows as wall
                    InternalBoardSquare wallSquare;
                    wallSquare.setType(WALL);
                    scan[i][j] = ExternalBoardSquare(wallSquare);
                }
            }
    }

        return scan;
    }

    /**
     * Helper method to keep track of remaning paint blobs 
     */
    int GameBoard::getRemainingPaintBlobs(RobotMoveRequest::RobotColor robot) const {
        return (robot == RobotMoveRequest::RED) ? redPaintBlobsRemaining : bluePaintBlobsRemaining;
    }
}
