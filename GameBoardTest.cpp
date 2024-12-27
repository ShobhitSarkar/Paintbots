// GameBoardTest.cpp
#include "GameBoardTest.h"
#include "GameBoard.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <cstring> 
#include <stdexcept>  
#include <algorithm>  

namespace Paintbots {

/**
 * @test Helper method to create config files 
 */
void GameBoardTest::createTestConfig(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    file << content;
    file.close();
}

/**
 * @test Helper method to get GameBoard instance 
 */
GameBoard* GameBoardTest::getTestBoard() {
    return GameBoard::getInstance("xyzzy");
}

/**
 * @test Verifies whether the board dimensions are correct 
 */
void GameBoardTest::verifyBoardDimensions(GameBoard* board) {
    assert(board != nullptr);
    for (int i = 0; i < 17; ++i) {
        assert(board->getSquare(0, i).getSquareType() == WALL);
        assert(board->getSquare(16, i).getSquareType() == WALL);
        assert(board->getSquare(i, 0).getSquareType() == WALL);
        assert(board->getSquare(i, 16).getSquareType() == WALL);
    }
}

/**
 * @test Verifies that an empty square is actually an empty square 
 */
void GameBoardTest::verifyEmptySquare(int row, int col) {
    GameBoard* board = getTestBoard();
    InternalBoardSquare& square = board->getSquare(row, col);
    assert(square.getSquareType() == EMPTY);
    assert(!square.redRobotPresent());
    assert(!square.blueRobotPresent());
    assert(square.getSquareColor() == WHITE);
}

/**
 * Helper method to check if a position is valid 
 */
bool GameBoardTest::isValidPosition(int row, int col) const {
    return row >= 0 && row < 17 && col >= 0 && col < 17;
}

/**
 * @test Test to get instance with the correct password 
 */
void GameBoardTest::testGetInstanceWithPassword() {
    GameBoard* board = GameBoard::getInstance("xyzzy");
    assert(board != nullptr);
    std::cout << "Get instance with correct password test: PASS\n";
}

/**
 * @test Testing program behavior when wrong password is supplied 
 */
void GameBoardTest::testGetInstanceWithWrongPassword() {
    try {
        GameBoard::getInstance("wrong");
        assert(false && "Should throw exception for incorrect password");
    } catch (const std::runtime_error& e) {
        std::cout << "Get instance with wrong password test: PASS\n";
    }
}

/**
 * @test Tests whether GameBoard is instantiated only once 
 */
void GameBoardTest::testSingletonBehavior() {
    GameBoard* board1 = GameBoard::getInstance("xyzzy");
    GameBoard* board2 = GameBoard::getInstance("xyzzy");
    assert(board1 == board2);
    std::cout << "Singleton behavior test: PASS\n";
}

/**
 * @test Tests program behavior when wrong password is supplied followed 
 * by the correct password 
 */
void GameBoardTest::testPasswordProtection() {
    try {
        // This should throw an exception
        GameBoard::getInstance("wrong");
        assert(false && "Should throw exception for incorrect password");
    } catch (const std::runtime_error& e) {
        // First part of test passes - incorrect password throws exception
    }

    try {
        // This should succeed
        GameBoard* board = GameBoard::getInstance("xyzzy");
        assert(board != nullptr);
        std::cout << "Password protection test: PASS\n";
    } catch (const std::runtime_error& e) {
        assert(false && "Correct password should not throw an exception");
    }
}

/**
 * @test Tests correct board dimentions 
 */
void GameBoardTest::testBoardDimensions() {
    GameBoard* board = getTestBoard();
    verifyBoardDimensions(board);
    std::cout << "Board dimensions test: PASS\n";
}

/**
 * @test Tests whether walls are placed correctly 
 */
void GameBoardTest::testWallPlacement() {
    GameBoard* board = getTestBoard();
    for (int i = 0; i < 17; ++i) {
        assert(board->getSquare(0, i).getSquareType() == WALL);
        assert(board->getSquare(16, i).getSquareType() == WALL);
        assert(board->getSquare(i, 0).getSquareType() == WALL);
        assert(board->getSquare(i, 16).getSquareType() == WALL);
    }
    std::cout << "Wall placement test: PASS\n";
}

/**
 * @test Tests whether rocks are placed correctly 
 */
void GameBoardTest::testRockPlacement() {
    GameBoard* board = getTestBoard();
    int rockCount = 0;
    for (int i = 1; i < 16; ++i) {
        for (int j = 1; j < 16; ++j) {
            if (board->getSquare(i, j).getSquareType() == ROCK) {
                rockCount++;
            }
        }
    }
    assert(rockCount >= Config::getInstance().getRockLowerBound());
    assert(rockCount <= Config::getInstance().getRockUpperBound());
    std::cout << "Rock placement test: PASS\n";
}

/**
 * @test Tests whether fog is placed correctly 
 */
void GameBoardTest::testFogPlacement() {
    GameBoard* board = getTestBoard();
    int fogCount = 0;
    for (int i = 1; i < 16; ++i) {
        for (int j = 1; j < 16; ++j) {
            if (board->getSquare(i, j).getSquareType() == FOG) {
                fogCount++;
            }
        }
    }
    assert(fogCount >= Config::getInstance().getFogLowerBound());
    assert(fogCount <= Config::getInstance().getFogUpperBound());
    std::cout << "Fog placement test: PASS\n";
}

/**
 * @test Tests whether robot is initially placed correctly 
 */
void GameBoardTest::testRobotInitialPlacement() {
    GameBoard* board = getTestBoard();
    bool redFound = false, blueFound = false;
    
    for (int i = 1; i < 16; ++i) {
        for (int j = 1; j < 16; ++j) {
            InternalBoardSquare& square = board->getSquare(i, j);
            if (square.redRobotPresent()) {
                assert(!redFound);  // Should only find one red robot
                redFound = true;
                assert(square.getSquareType() == EMPTY);
            }
            if (square.blueRobotPresent()) {
                assert(!blueFound);  // Should only find one blue robot
                blueFound = true;
                assert(square.getSquareType() == EMPTY);
            }
        }
    }
    assert(redFound && blueFound);
    std::cout << "Robot initial placement test: PASS\n";
}

/**
 * @test Tests whether robot is placed correctly and then validated 
 */
void GameBoardTest::testRobotPlacementValidation() {
    GameBoard* board = getTestBoard();
    bool robotsOnSameSquare = false;
    bool robotOnRock = false;
    bool robotInFog = false;
    
    for (int i = 1; i < 16; ++i) {
        for (int j = 1; j < 16; ++j) {
            InternalBoardSquare& square = board->getSquare(i, j);
            if (square.redRobotPresent() && square.blueRobotPresent()) {
                robotsOnSameSquare = true;
            }
            if ((square.redRobotPresent() || square.blueRobotPresent()) && 
                square.getSquareType() == ROCK) {
                robotOnRock = true;
            }
            if ((square.redRobotPresent() || square.blueRobotPresent()) && 
                square.getSquareType() == FOG) {
                robotInFog = true;
            }
        }
    }
    
    assert(!robotsOnSameSquare);
    assert(!robotOnRock);
    assert(!robotInFog);
    std::cout << "Robot placement validation test: PASS\n";
}

/**
 * @test Tests basic robot movement: Forward, Backward, RotateRight and 
 * RotateLeft 
 */
void GameBoardTest::testBasicRobotMovements() {
    GameBoard* board = getTestBoard();
    
    // First clear any existing robots
    for (int i = 1; i < 16; ++i) {
        for (int j = 1; j < 16; ++j) {
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
        }
    }
    
    // Place red robot in a known good position (center of board)
    InternalBoardSquare& square = board->getSquare(8, 8);
    square.setRedRobot(true);
    square.setRobotDirection(NORTH);  // Face north so we know there's room to move
    
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.shoot = false;
    
    // Try forward movement
    request.move = RobotMoveRequest::FORWARD;
    bool forwardResult = board->moveRobot(request);
    if (!forwardResult) {
        std::cout << "Forward movement failed" << std::endl;
    }
    assert(forwardResult);
    
    // Try left rotation
    request.move = RobotMoveRequest::ROTATELEFT;
    bool leftResult = board->moveRobot(request);
    if (!leftResult) {
        std::cout << "Left rotation failed" << std::endl;
    }
    assert(leftResult);
    
    // Try right rotation
    request.move = RobotMoveRequest::ROTATERIGHT;
    bool rightResult = board->moveRobot(request);
    if (!rightResult) {
        std::cout << "Right rotation failed" << std::endl;
    }
    assert(rightResult);
    
    std::cout << "Basic robot movements test: PASS\n";
}   

/**
 * @test Tests program behavior when robot hits a wall 
 */
void GameBoardTest::testWallCollisionPrevention() {
    GameBoard* board = getTestBoard();
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.move = RobotMoveRequest::FORWARD;
    
    // Move robot to wall edge
    while (board->moveRobot(request)) {}
    
    // Try to move into wall
    assert(!board->moveRobot(request));
    std::cout << "Wall collision prevention test: PASS\n";
}

/**
 * @test Tests program behavior when robot is trying to hit rock 
 */
void GameBoardTest::testRockCollisionPrevention() {
    GameBoard* board = getTestBoard();
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.move = RobotMoveRequest::FORWARD;
    
    // Reset board to known state
    for (int i = 1; i <= 15; ++i) {
        for (int j = 1; j <= 15; ++j) {
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
            board->getSquare(i, j).setType(EMPTY);
        }
    }
    
    // Manually create a rock and adjacent empty square
    int rockRow = 10, rockCol = 10;
    board->getSquare(rockRow, rockCol).setType(ROCK);
    board->getSquare(rockRow-1, rockCol).setType(EMPTY);
    
    // Place red robot just before the rock
    board->getSquare(rockRow-1, rockCol).setRedRobot(true);
    board->getSquare(rockRow-1, rockCol).setRobotDirection(SOUTH);
    
    // Attempt to move into rock square
    bool moveResult = board->moveRobot(request);
    
    // Debug output
    std::cout << "Rock Position: (" << rockRow << ", " << rockCol << ")" << std::endl;
    std::cout << "Robot Initial Position: (" << rockRow-1 << ", " << rockCol << ")" << std::endl;
    std::cout << "Move Result: " << (moveResult ? "TRUE" : "FALSE") << std::endl;
    
    // Assert that move fails when encountering a rock
    assert(!moveResult);
    std::cout << "Rock collision prevention test: PASS\n";
}

/**
 * @test Testing movement when fog is moving through the board 
 */
void GameBoardTest::testFogMovement() {
    // Reset the board to a known state
    GameBoard* board = getTestBoard();
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            board->getSquare(i, j).setType(EMPTY);
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
            board->getSquare(i, j).setSquareColor(WHITE);
        }
    }

    // Test Scenario 1: Robot can move through fog
    {
        // Setup: Fog square with empty square ahead
        board->getSquare(10, 10).setType(FOG);
        board->getSquare(9, 10).setRedRobot(true);
        board->getSquare(9, 10).setRobotDirection(SOUTH);

        RobotMoveRequest request;
        request.robot = RobotMoveRequest::RED;
        request.move = RobotMoveRequest::FORWARD;

        bool moveResult = board->moveRobot(request);
        assert(moveResult == true);
        assert(board->getSquare(10, 10).redRobotPresent());
        assert(!board->getSquare(9, 10).redRobotPresent());
        std::cout << "Fog movement test (through fog): PASS\n";
    }

    // Reset board
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            board->getSquare(i, j).setType(EMPTY);
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
            board->getSquare(i, j).setSquareColor(WHITE);
        }
    }

    // Test Scenario 2: Robot in fog square gets limited vision
    {
        // Setup: Robot in fog square
        board->getSquare(10, 10).setType(FOG);
        board->getSquare(10, 10).setRedRobot(true);
        board->getSquare(10, 10).setRobotDirection(SOUTH);

        // Get short-range scan
        ExternalBoardSquare** scan = board->getShortRangeScan(RobotMoveRequest::RED);

        // Verify scan conditions
        // According to spec: "when in a fog square, the 5x5 scan is all empty except center"
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                if (i == 2 && j == 2) {
                    // Center square should be fog
                    assert(scan[i][j].getSquareType() == FOG);
                } else {
                    // All other squares should be empty
                    assert(scan[i][j].getSquareType() == EMPTY);
                }
            }
        }

        // Clean up scan
        for (int i = 0; i < 5; ++i) {
            delete[] scan[i];
        }
        delete[] scan;

        std::cout << "Fog vision limitation test: PASS\n";
    }
}

/**
 * @test Testing what happens when robot hits boundary (wall)
 */
void GameBoardTest::testBoardBoundaryPrevention() {
    GameBoard* board = getTestBoard();
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.move = RobotMoveRequest::FORWARD;
    
    // Find initial robot position
    int initialRow = -1, initialCol = -1;
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            if (board->getSquare(i, j).redRobotPresent()) {
                initialRow = i;
                initialCol = j;
                // Set robot's direction to ensure it moves toward a boundary
                board->getSquare(i, j).setRobotDirection(NORTH);
                std::cout << "Found red robot at position (" << i << "," << j << ")\n";
                break;
            }
        }
        if (initialRow != -1) break;
    }
    
    // Ensure we found the robot
    assert(initialRow != -1);
    assert(initialCol != -1);
    
    // Move robot to boundary
    int moveCount = 0;
    while (board->moveRobot(request)) {
        moveCount++;
        std::cout << "Move " << moveCount << " successful\n";
        // Print current position for debugging
        for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
            for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
                if (board->getSquare(i, j).redRobotPresent()) {
                    std::cout << "Robot now at position (" << i << "," << j << ")\n";
                    break;
                }
            }
        }
    }
    
    // Try to move beyond boundary
    bool beyondBoundaryMove = board->moveRobot(request);
    
    assert(!beyondBoundaryMove);
    std::cout << "Board boundary prevention test: PASS\n";
}

/**
 * @test Tests whether robots are correctly painting squares 
 */
void GameBoardTest::testColorTrails() {
    GameBoard* board = getTestBoard();
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.move = RobotMoveRequest::FORWARD;
    
    // Find initial robot position
    int initialRow = -1, initialCol = -1;
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            if (board->getSquare(i, j).redRobotPresent()) {
                initialRow = i;
                initialCol = j;
                break;
            }
        }
        if (initialRow != -1) break;
    }
    
    // Ensure we found the robot
    assert(initialRow != -1);

    // Debug print initial state
    std::cout << "Initial Square Color: " 
              << board->getSquare(initialRow, initialCol).getSquareColor() << std::endl;
    std::cout << "Initial Robot Position: (" 
              << initialRow << ", " << initialCol << ")" << std::endl;
    
    // Store the initial square's color before moving
    SquareColor initialColor = board->getSquare(initialRow, initialCol).getSquareColor();
    
    // Find next square based on robot's direction
    Direction robotDir = board->getSquare(initialRow, initialCol).robotDirection();
    int nextRow = initialRow, nextCol = initialCol;
    
    switch(robotDir) {
        case NORTH: nextRow--; break;
        case SOUTH: nextRow++; break;
        case EAST: nextCol++; break;
        case WEST: nextCol--; break;
    }
    
    std::cout << "Robot Direction: " << robotDir << std::endl;
    std::cout << "Next Square Position: (" 
              << nextRow << ", " << nextCol << ")" << std::endl;
    std::cout << "Next Square Type: " 
              << board->getSquare(nextRow, nextCol).getSquareType() << std::endl;
    
    // Attempt move
    bool moveResult = board->moveRobot(request);
    
    std::cout << "Move Result: " << (moveResult ? "TRUE" : "FALSE") << std::endl;
    
    // Check color after move
    SquareColor currentColor = board->getSquare(initialRow, initialCol).getSquareColor();
    std::cout << "Current Square Color After Move: " << currentColor << std::endl;
    
    // Only assert color change if move was successful and square wasn't already red
    if (moveResult && initialColor != RED) {
        assert(currentColor == RED);
    }
    
    std::cout << "Color trails test: PASS\n";
}

/**
 * @test Testing program behavior when two robots collide 
 */
void GameBoardTest::testRobotCollisionPrevention() {
    GameBoard* board = getTestBoard();
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.move = RobotMoveRequest::FORWARD;
    
    // Position robots adjacent to each other
    int robotsFound = 0;
    for (int i = 1; i < 15; ++i) {
        for (int j = 1; j < 15; ++j) {
            if (board->getSquare(i, j).getSquareType() == EMPTY && 
                board->getSquare(i+1, j).getSquareType() == EMPTY) {
                // Found two adjacent empty squares
                board->getSquare(i, j).setRedRobot(true);
                board->getSquare(i+1, j).setBlueRobot(true);
                robotsFound = 1;
                break;
            }
        }
        if (robotsFound) break;
    }
    
    // Try to move red robot into blue robot's square
    assert(!board->moveRobot(request));
    std::cout << "Robot collision prevention test: PASS\n";
}

/**
 * @test Testing program behavior when a robot is hit with a paint blob 
 */
void GameBoardTest::testPaintBlobHit() {
    GameBoard* board = getTestBoard();
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.shoot = true;
    request.move = RobotMoveRequest::NONE;
    
    // Reset board to known state
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
            board->getSquare(i, j).setType(EMPTY);
        }
    }
    
    // Place red robot and set its direction
    board->getSquare(8, 8).setRedRobot(true);
    board->getSquare(8, 8).setRobotDirection(NORTH);
    
    // Place blue robot in front of red robot
    board->getSquare(7, 8).setBlueRobot(true);
    
    // Debug output
    std::cout << "Red robot at (8,8) facing NORTH\n";
    std::cout << "Blue robot at (7,8)\n";
    std::cout << "Paint blob hit result: " << (board->paintBlobHit(request) ? "true" : "false") << "\n";
    
    assert(board->paintBlobHit(request));
    std::cout << "Paint blob hit test: PASS\n";
}

/**
 * @test Test when robot shooting a paint blob misses 
 */
void GameBoardTest::testPaintBlobMiss() {
    GameBoard* board = getTestBoard();
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.shoot = true;
    request.move = RobotMoveRequest::NONE;
    
    // Place robots not in line of sight
    for (int i = 1; i < 15; ++i) {
        for (int j = 1; j < 15; ++j) {
            if (board->getSquare(i, j).getSquareType() == EMPTY) {
                board->getSquare(i, j).setRedRobot(true);
                board->getSquare(i+1, j+1).setBlueRobot(true);  // Diagonal placement
                assert(!board->paintBlobHit(request));
                std::cout << "Paint blob miss test: PASS\n";
                return;
            }
        }
    }
}

/**
 * @test Tests when there's a rock in between the line of fire of paint blobs 
 */
void GameBoardTest::testPaintBlobRockBlocking() {
    GameBoard* board = getTestBoard();
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.shoot = true;
    request.move = RobotMoveRequest::NONE;
    
    // Find a rock
    for (int i = 1; i < 15; ++i) {
        for (int j = 1; j < 15; ++j) {
            if (board->getSquare(i, j).getSquareType() == ROCK) {
                // Place red robot before rock
                if (i > 1) {
                    board->getSquare(i-1, j).setRedRobot(true);
                    // Place blue robot after rock
                    board->getSquare(i+1, j).setBlueRobot(true);
                    assert(!board->paintBlobHit(request));
                    std::cout << "Paint blob rock blocking test: PASS\n";
                    return;
                }
            }
        }
    }
}

/**
 * @test Testing program behavior when firing paint blob through fog 
 */
void GameBoardTest::testPaintBlobFogBehavior() {
    GameBoard* board = getTestBoard();
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.shoot = true;
    request.move = RobotMoveRequest::NONE;
    
    // Reset board to a known state
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
            board->getSquare(i, j).setType(EMPTY);
        }
    }
    
    // Manually create fog and robot positions for a clear test scenario
    int fogRow = 10, fogCol = 10;
    board->getSquare(fogRow, fogCol).setType(FOG);
    
    // Place red robot before fog
    board->getSquare(fogRow - 1, fogCol).setRedRobot(true);
    board->getSquare(fogRow - 1, fogCol).setRobotDirection(SOUTH);
    
    // Place blue robot after fog
    board->getSquare(fogRow + 1, fogCol).setBlueRobot(true);
    
    // Diagnostic information
    std::cout << "Fog Position: (" << fogRow << ", " << fogCol << ")" << std::endl;
    std::cout << "Red Robot Position: (" << fogRow - 1 << ", " << fogCol << ")" << std::endl;
    std::cout << "Blue Robot Position: (" << fogRow + 1 << ", " << fogCol << ")" << std::endl;
    
    // Check paint blob behavior
    bool paintBlobResult = board->paintBlobHit(request);
    
    // Debug output
    std::cout << "Paint Blob Hit Result: " << (paintBlobResult ? "TRUE" : "FALSE") << std::endl;
    
    // Assert that paint blob goes through fog
    assert(paintBlobResult);
    std::cout << "Paint blob fog behavior test: PASS\n";
}

/**
 * @test Testing program behavior when paint blob hits a wall 
 */
void GameBoardTest::testPaintBlobWallCollision() {
    GameBoard* board = getTestBoard();
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.shoot = true;
    request.move = RobotMoveRequest::NONE;
    
    // Place red robot next to wall
    board->getSquare(1, 1).setRedRobot(true);
    assert(!board->paintBlobHit(request));
    std::cout << "Paint blob wall collision test: PASS\n";
}

/**
 * @test Testing if paintblob is actually moving 
 */
void GameBoardTest::testPaintBlobRange() {
    GameBoard* board = getTestBoard();
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.shoot = true;
    request.move = RobotMoveRequest::NONE;
    
    // Reset board to a known state
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
            board->getSquare(i, j).setType(EMPTY);
        }
    }
    
    // Manually create a clear line scenario
    int lineRow = 10;  // Choose a specific row for testing
    
    // Place red robot at the start of the line
    board->getSquare(lineRow, 1).setRedRobot(true);
    board->getSquare(lineRow, 1).setRobotDirection(EAST);
    
    // Place blue robot at the end of the line
    board->getSquare(lineRow, GameBoard::BOARD_SIZE).setBlueRobot(true);
    
    // Diagnostic information
    std::cout << "Red Robot Position: (" << lineRow << ", 1)" << std::endl;
    std::cout << "Blue Robot Position: (" << lineRow << ", " << GameBoard::BOARD_SIZE << ")" << std::endl;
    
    // Check paint blob behavior
    bool paintBlobResult = board->paintBlobHit(request);
    
    // Debug output
    std::cout << "Paint Blob Hit Result: " << (paintBlobResult ? "TRUE" : "FALSE") << std::endl;
    
    // Assert that paint blob hits the target
    assert(paintBlobResult);
    std::cout << "Paint blob range test: PASS\n";
}

/**
 * @test Testing whether paintblob moves in the correct direction 
 */
void GameBoardTest::testPaintBlobDirection() {
    GameBoard* board = getTestBoard();
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.shoot = true;
    request.move = RobotMoveRequest::ROTATELEFT;
    
    // Place red robot at center
    board->getSquare(7, 7).setRedRobot(true);
    
    // Test each direction
    for (int i = 0; i < 4; ++i) {
        // Move blue robot to be in line with new direction
        switch(i) {
            case 0: board->getSquare(6, 7).setBlueRobot(true); break; // North
            case 1: board->getSquare(7, 6).setBlueRobot(true); break; // West
            case 2: board->getSquare(8, 7).setBlueRobot(true); break; // South
            case 3: board->getSquare(7, 8).setBlueRobot(true); break; // East
        }
        
        // Verify hit detection works
        assert(board->paintBlobHit(request));
        
        // Clear blue robot's position for next test
        switch(i) {
            case 0: board->getSquare(6, 7).setBlueRobot(false); break;
            case 1: board->getSquare(7, 6).setBlueRobot(false); break;
            case 2: board->getSquare(8, 7).setBlueRobot(false); break;
            case 3: board->getSquare(7, 8).setBlueRobot(false); break;
        }
        
        board->moveRobot(request); // Rotate for next test
    }
    std::cout << "Paint blob direction test: PASS\n";
}

/**
 * @test Testing whether initial scores are 0 
 */
void GameBoardTest::testInitialScore() {
    GameBoard* board = getTestBoard();
    
    assert(board->redScore() == 0);
    assert(board->blueScore() == 0);
    std::cout << "Initial score test: PASS\n";
}

/**
 * @test Testing if scores increase correctly 
 */
void GameBoardTest::testScoreIncrementOnMovement() {
    GameBoard* board = getTestBoard();
    
    // Reset board to a known state
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
            board->getSquare(i, j).setSquareColor(WHITE);
        }
    }
    
    // Place a red robot
    board->getSquare(8, 8).setRedRobot(true);
    board->getSquare(8, 8).setRobotDirection(NORTH);
    
    // Initial score
    int initialRedScore = board->redScore();
    
    // Perform a move that should paint a square
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.move = RobotMoveRequest::FORWARD;
    request.shoot = false;
    
    board->moveRobot(request);
    
    // Check that the score has increased
    int newRedScore = board->redScore();
    assert(newRedScore > initialRedScore);
    std::cout << "Score increment on movement test: PASS\n";
}

/**
 * @test Testing square changes when a robot gets hit by a paint blob 
 */
void GameBoardTest::testScoreUpdateOnPaintBlob() {
    GameBoard* board = getTestBoard();
    
    // Reset board
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
            board->getSquare(i, j).setSquareColor(WHITE);
            board->getSquare(i, j).setType(EMPTY);
        }
    }
    
    // Place robots
    board->getSquare(8, 8).setRedRobot(true);
    board->getSquare(8, 8).setRobotDirection(NORTH);
    board->getSquare(7, 8).setBlueRobot(true);
    board->getSquare(7, 8).setRobotDirection(NORTH);
    
    std::cout << "Initial board state:\n";
    std::cout << "Red robot at (8,8) facing NORTH\n";
    std::cout << "Blue robot at (7,8) facing NORTH\n";
    
    int initialRedScore = board->redScore();
    std::cout << "Initial red score: " << initialRedScore << "\n";
    
    // Simulate paint blob hit
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.move = RobotMoveRequest::NONE;
    request.shoot = true;
    
    bool hitResult = board->paintBlobHit(request);
    std::cout << "Paint blob hit result: " << (hitResult ? "true" : "false") << "\n";
    
    board->setRobotPaintColor(RobotMoveRequest::BLUE, RED);
    std::cout << "Set blue robot to paint red\n";
    
    // Move blue robot
    request.robot = RobotMoveRequest::BLUE;
    request.move = RobotMoveRequest::FORWARD;
    request.shoot = false;
    
    bool moveResult = board->moveRobot(request);
    std::cout << "Blue robot move result: " << (moveResult ? "true" : "false") << "\n";
    
    // Print colors of all squares to verify state
    for (int i = 6; i <= 9; ++i) {
        for (int j = 7; j <= 9; ++j) {
            std::cout << "Square (" << i << "," << j << ") color: " 
                      << board->getSquare(i, j).getSquareColor() << "\n";
        }
    }
    
    int newRedScore = board->redScore();
    std::cout << "New red score: " << newRedScore << "\n";
    
    assert(newRedScore > initialRedScore);
}

/**
 * @test Testing scoring after multiple operations on robots 
 */
void GameBoardTest::testScoreAfterMultipleOperations() {
    GameBoard* board = getTestBoard();
    
    // Reset board to a known state
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
            board->getSquare(i, j).setSquareColor(WHITE);
        }
    }
    
    // Place red robot
    board->getSquare(8, 8).setRedRobot(true);
    board->getSquare(8, 8).setRobotDirection(NORTH);
    
    // Perform multiple moves and checks
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.shoot = false;
    
    int initialRedScore = board->redScore();
    
    // Move multiple times
    for (int i = 0; i < 5; ++i) {
        request.move = RobotMoveRequest::FORWARD;
        board->moveRobot(request);
        
        request.move = RobotMoveRequest::ROTATELEFT;
        board->moveRobot(request);
    }
    
    int finalRedScore = board->redScore();
    
    // Verify that score has increased
    assert(finalRedScore > initialRedScore);
    std::cout << "Score after multiple operations test: PASS\n";
}

/**
 * @test Testing is short range scan is working properly 
 */
void GameBoardTest::testShortRangeScanBasic() {
    GameBoard* board = getTestBoard();
    
    // Reset board to a known state
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
            board->getSquare(i, j).setType(EMPTY);
        }
    }
    
    // Place red robot in a known position
    board->getSquare(8, 8).setRedRobot(true);
    board->getSquare(8, 8).setRobotDirection(NORTH);
    
    // Perform short-range scan
    ExternalBoardSquare** scan = board->getShortRangeScan(RobotMoveRequest::RED);
    
    // Verify scan dimensions
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            assert(scan[i][j].getSquareType() != WALL);
        }
    }
    
    // Verify center square has the robot
    assert(scan[2][2].redRobotPresent());
    assert(scan[2][2].robotDirection() == NORTH);
    
    // Clean up
    for (int i = 0; i < 5; ++i) {
        delete[] scan[i];
    }
    delete[] scan;
    
    std::cout << "Short Range Scan Basic Test: PASS\n";
}

/**
 * @test Testing whether a short range scan is correct with respect to the position 
 * of the robot 
 */
void GameBoardTest::testShortRangeScanRobotPosition() {
    GameBoard* board = getTestBoard();
    
    struct TestCase {
        int row;
        int col;
        Direction dir;
        
        static void setup(GameBoard* board, int row, int col, Direction dir) {
            board->getSquare(row, col).setRedRobot(true);
            board->getSquare(row, col).setRobotDirection(dir);
        }
    } testCases[] = {
        {8, 8, NORTH},   // Center of board
        {2, 2, WEST},    // Near top-left corner
        {15, 15, EAST},  // Near bottom-right corner
    };
    
    for (const auto& testCase : testCases) {
        // Reset board
        for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
            for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
                board->getSquare(i, j).setRedRobot(false);
                board->getSquare(i, j).setType(EMPTY);
            }
        }
    
        TestCase::setup(board, testCase.row, testCase.col, testCase.dir);
        
        ExternalBoardSquare** scan = board->getShortRangeScan(RobotMoveRequest::RED);
        
        assert(scan[2][2].redRobotPresent());
        assert(scan[2][2].robotDirection() == testCase.dir);
        
        for (int i = 0; i < 5; ++i) {
            delete[] scan[i];
        }
        delete[] scan;
    }
    
    std::cout << "Short Range Scan Robot Position Test: PASS\n";
}

/**
 * @test Test short range scan for correct directions 
 */
void GameBoardTest::testShortRangeScanDirections() {
    GameBoard* board = getTestBoard();
    
    // Reset board
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
            board->getSquare(i, j).setType(EMPTY);
        }
    }
    
    // Place red robot in center
    board->getSquare(8, 8).setRedRobot(true);
    
    // Test each direction
    Direction directions[] = {NORTH, EAST, SOUTH, WEST};
    
    for (Direction dir : directions) {
        // Reset robot's direction
        board->getSquare(8, 8).setRobotDirection(dir);
        
        // Perform scan
        ExternalBoardSquare** scan = board->getShortRangeScan(RobotMoveRequest::RED);
        
        // Verify center square
        assert(scan[2][2].redRobotPresent());
        assert(scan[2][2].robotDirection() == dir);
        
        // Clean up
        for (int i = 0; i < 5; ++i) {
            delete[] scan[i];
        }
        delete[] scan;
    }
    
    std::cout << "Short Range Scan Directions Test: PASS\n";
}

/**
 * @test Testing short scan behavior to see whether walls show up 
 */
void GameBoardTest::testShortRangeScanWallBoundaries() {
    GameBoard* board = getTestBoard();
    
    int boundaryPositions[] = {1, 2, 14, 15};
    
    for (int row : boundaryPositions) {
        for (int col : boundaryPositions) {
            for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
                for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
                    board->getSquare(i, j).setRedRobot(false);
                    board->getSquare(i, j).setType(EMPTY);
                }
            }
            
            board->getSquare(row, col).setRedRobot(true);
            board->getSquare(row, col).setRobotDirection(NORTH);
            
            ExternalBoardSquare** scan = board->getShortRangeScan(RobotMoveRequest::RED);
            
            int wallCount = 0;
            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < 5; ++j) {
                    if (scan[i][j].getSquareType() == WALL) {
                        wallCount++;
                    }
                }
            }
            
            assert(wallCount > 0);
            
            for (int i = 0; i < 5; ++i) {
                delete[] scan[i];
            }
            delete[] scan;
        }
    }
    
    std::cout << "Short Range Scan Wall Boundaries Test: PASS\n";
}

/**
 * @test Testing whether fog shows up on short range scan 
 */
void GameBoardTest::testShortRangeScanFogBehavior() {
    GameBoard* board = getTestBoard();
    
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
            board->getSquare(i, j).setType(EMPTY);
        }
    }
    
    board->getSquare(8, 8).setRedRobot(true);
    board->getSquare(8, 8).setType(FOG);
    board->getSquare(8, 8).setRobotDirection(NORTH);
    
    ExternalBoardSquare** scan = board->getShortRangeScan(RobotMoveRequest::RED);
    
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i == 2 && j == 2) {
                assert(scan[i][j].getSquareType() == FOG);
                assert(scan[i][j].redRobotPresent());
            } else {
                assert(scan[i][j].getSquareType() == EMPTY);
            }
        }
    }
    
    for (int i = 0; i < 5; ++i) {
        delete[] scan[i];
    }
    delete[] scan;
    
    std::cout << "Short Range Scan Fog Behavior Test: PASS\n";
}

/**
 * @test Testing when robot is surrounded by obstacles 
 */
void GameBoardTest::testShortRangeScanObstructions() {
    GameBoard* board = getTestBoard();
    
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
            board->getSquare(i, j).setType(EMPTY);
        }
    }
    
    board->getSquare(8, 8).setRedRobot(true);
    board->getSquare(8, 8).setRobotDirection(NORTH);
    
    board->getSquare(7, 8).setType(ROCK);  // Rock in front
    board->getSquare(8, 7).setType(ROCK);  // Rock to the left
    board->getSquare(9, 8).setBlueRobot(true);  // Blue robot below
    
    ExternalBoardSquare** scan = board->getShortRangeScan(RobotMoveRequest::RED);
    
    std::cout << "Scan Contents:\n";
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            std::cout << "scan[" << i << "][" << j << "] - Type: " 
                      << scan[i][j].getSquareType() 
                      << ", Robot: " 
                      << (scan[i][j].redRobotPresent() ? "Red" : 
                          (scan[i][j].blueRobotPresent() ? "Blue" : "None"))
                      << "\n";
        }
    }
    
    bool rockFoundInFront = false;
    bool blueRobotFound = false;
    
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (scan[i][j].getSquareType() == ROCK) {
                std::cout << "Rock found at [" << i << "][" << j << "]\n";
                rockFoundInFront = true;
            }
            if (scan[i][j].blueRobotPresent()) {
                std::cout << "Blue robot found at [" << i << "][" << j << "]\n";
                blueRobotFound = true;
            }
        }
    }
    
    assert(rockFoundInFront);
    assert(blueRobotFound);
    
    for (int i = 0; i < 5; ++i) {
        delete[] scan[i];
    }
    delete[] scan;
    
    std::cout << "Short Range Scan Obstructions Test: PASS\n";
}

/**
 * @test Testing long range scan basics 
 */
void GameBoardTest::testLongRangeDimensions() {
    GameBoard* board = getTestBoard();
    ExternalBoardSquare** scan = board->getLongRangeScan();
    
    bool validDimensions = true;
    for (int i = 0; i < 17; ++i) {
        for (int j = 0; j < 17; ++j) {
            if (&scan[i][j] == nullptr) {
                validDimensions = false;
                break;
            }
        }
    }
    
    assert(validDimensions);
    
    for (int i = 0; i < 17; ++i) {
        delete[] scan[i];
    }
    delete[] scan;
    
    std::cout << "Long range dimensions test: PASS\n";
}

/**
 * @test Testing whether walls show up on long range scans 
 */
void GameBoardTest::testLongRangeWalls() {
    GameBoard* board = getTestBoard();
    ExternalBoardSquare** scan = board->getLongRangeScan();
    
    // Check walls on edges
    for (int i = 0; i < 17; ++i) {
        assert(scan[0][i].getSquareType() == WALL);
        assert(scan[16][i].getSquareType() == WALL);
        assert(scan[i][0].getSquareType() == WALL);
        assert(scan[i][16].getSquareType() == WALL);
    }
    
    // Cleanup
    for (int i = 0; i < 17; ++i) {
        delete[] scan[i];
    }
    delete[] scan;
    
    std::cout << "Long range walls test: PASS\n";
}

/**
 * @test Testing if rocks show up on long range scans 
 */
void GameBoardTest::testLongRangeRocks() {
    GameBoard* board = getTestBoard();
    ExternalBoardSquare** scan = board->getLongRangeScan();
    
    int scanRocks = 0, boardRocks = 0;
    
    for (int i = 1; i < 16; ++i) {
        for (int j = 1; j < 16; ++j) {
            if (scan[i][j].getSquareType() == ROCK) {
                scanRocks++;
            }
            if (board->getSquare(i, j).getSquareType() == ROCK) {
                boardRocks++;
            }
        }
    }
    
    assert(scanRocks == boardRocks);
    
    for (int i = 0; i < 17; ++i) {
        delete[] scan[i];
    }
    delete[] scan;
    
    std::cout << "Long range rocks test: PASS\n";
}

/**
 * @test Tests whether fog shows up on long range scans 
 */
void GameBoardTest::testLongRangeFog() {
    GameBoard* board = getTestBoard();
    ExternalBoardSquare** scan = board->getLongRangeScan();
    
    // Count fog squares and compare with actual board
    int scanFog = 0, boardFog = 0;
    
    for (int i = 1; i < 16; ++i) {
        for (int j = 1; j < 16; ++j) {
            if (scan[i][j].getSquareType() == FOG) {
                scanFog++;
            }
            if (board->getSquare(i, j).getSquareType() == FOG) {
                boardFog++;
            }
        }
    }
    
    assert(scanFog == boardFog);
    
    // Cleanup
    for (int i = 0; i < 17; ++i) {
        delete[] scan[i];
    }
    delete[] scan;
    
    std::cout << "Long range fog test: PASS\n";
}

/**
 * @test Tests whether robots correctly show up on long range scan 
 */
void GameBoardTest::testLongRangeRobots() {
    GameBoard* board = getTestBoard();
    ExternalBoardSquare** scan = board->getLongRangeScan();
    
    bool redFound = false, blueFound = false;
    bool correctRedPos = true, correctBluePos = true;
    
    for (int i = 1; i < 16; ++i) {
        for (int j = 1; j < 16; ++j) {
            if (scan[i][j].redRobotPresent() != board->getSquare(i, j).redRobotPresent()) {
                correctRedPos = false;
            }
            if (scan[i][j].blueRobotPresent() != board->getSquare(i, j).blueRobotPresent()) {
                correctBluePos = false;
            }
            redFound |= scan[i][j].redRobotPresent();
            blueFound |= scan[i][j].blueRobotPresent();
        }
    }
    
    assert(redFound && blueFound && correctRedPos && correctBluePos);
    
    for (int i = 0; i < 17; ++i) {
        delete[] scan[i];
    }
    delete[] scan;
    
    std::cout << "Long range robots test: PASS\n";
}

/**
 * @test Testing whether colors show up correctly on the long range scans 
 */
void GameBoardTest::testLongRangeColors() {
    GameBoard* board = getTestBoard();
    ExternalBoardSquare** scan = board->getLongRangeScan();
    
    bool colorsMatch = true;
    
    for (int i = 0; i < 17; ++i) {
        for (int j = 0; j < 17; ++j) {
            if (scan[i][j].getSquareColor() != board->getSquare(i, j).getSquareColor()) {
                colorsMatch = false;
                break;
            }
        }
    }
    
    assert(colorsMatch);
    
    for (int i = 0; i < 17; ++i) {
        delete[] scan[i];
    }
    delete[] scan;
    
    std::cout << "Long range colors test: PASS\n";
}

/**
 * @test Tests whether subscribers are notified when state changes 
 */
void GameBoardTest::testMovementNotification() {
    GameBoard* board = getTestBoard();
    MockObserver observer;
    board->addObserver(&observer);
    
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setType(EMPTY);
        }
    }
    
    board->getSquare(8, 8).setRedRobot(true);
    board->getSquare(8, 8).setRobotDirection(NORTH);
    
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.move = RobotMoveRequest::FORWARD;
    
    int initialCount = observer.updateCount;
    bool moveResult = board->moveRobot(request);
    
    if (moveResult) {
        assert(observer.updateCount > initialCount);
        std::cout << "Movement notification test: PASS\n";
    } else {
        std::cout << "Failed to move robot - skipping notification check\n";
        throw std::runtime_error("Robot move failed in testMovementNotification");
    }
    
    board->removeObserver(&observer);
}

/**
 * @test tests whether color changes triggers notification 
 */
void GameBoardTest::testColorChangeNotification() {
    GameBoard* board = getTestBoard();
    MockObserver observer;
    board->addObserver(&observer);
    
    // Find an empty square and change its color
    for (int i = 1; i < 16; ++i) {
        for (int j = 1; j < 16; ++j) {
            if (board->getSquare(i, j).getSquareType() == EMPTY) {
                int initialCount = observer.updateCount;
                board->setSquareColor(i, j, RED);
                assert(observer.updateCount > initialCount);
                board->removeObserver(&observer);
                std::cout << "Color change notification test: PASS\n";
                return;
            }
        }
    }
}

/**
 * @test Tests whether score changes trigger notifications to subscribers 
 */
void GameBoardTest::testScoreChangeNotification() {
    GameBoard* board = getTestBoard();
    MockObserver observer;
    board->addObserver(&observer);
    
    int initialCount = observer.updateCount;
    board->redScore();  // This should trigger notification
    assert(observer.updateCount > initialCount);
    
    board->removeObserver(&observer);
    std::cout << "Score change notification test: PASS\n";
}

/**
 * @test Testing whether subscriber actually gets the notification 
 */
void GameBoardTest::testObserverManagement() {
    GameBoard* board = getTestBoard();
    MockObserver observer1, observer2;
    
    std::cout << "Initial observer counts - observer1: " << observer1.updateCount 
              << ", observer2: " << observer2.updateCount << "\n";
    
    board->addObserver(&observer1);
    board->addObserver(&observer2);
    board->removeObserver(&observer1);
    
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.move = RobotMoveRequest::FORWARD;
    
    // Add debug output to verify robot exists and movement succeeds
    bool robotFound = false;
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            if (board->getSquare(i, j).redRobotPresent()) {
                robotFound = true;
                std::cout << "Found red robot at position (" << i << "," << j << ")\n";
                break;
            }
        }
        if (robotFound) break;
    }
    
    if (!robotFound) {
        std::cout << "No red robot found on board!\n";
    }
    
    bool moveResult = board->moveRobot(request);
    std::cout << "Move result: " << (moveResult ? "success" : "failure") << "\n";
    
    std::cout << "Final observer counts - observer1: " << observer1.updateCount 
              << ", observer2: " << observer2.updateCount << "\n";
    
    assert(observer1.updateCount == 0);
    assert(observer2.updateCount > 0);
}

/**
 * @test Testing whether multiple observers get notifications 
 */
void GameBoardTest::testMultipleObservers() {
    GameBoard* board = getTestBoard();
    MockObserver observer1, observer2, observer3;
    
    std::cout << "Adding observers...\n"; 
    board->addObserver(&observer1);
    board->addObserver(&observer2);
    board->addObserver(&observer3);
    
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.move = RobotMoveRequest::FORWARD;
    
    std::cout << "Making a move...\n"; 
    board->moveRobot(request);

    std::cout << "Observer counts - 1: " << observer1.updateCount 
              << ", 2: " << observer2.updateCount 
              << ", 3: " << observer3.updateCount << "\n";

    assert(observer1.updateCount > 0);
    assert(observer2.updateCount > 0);
    assert(observer3.updateCount > 0);
    
    std::cout << "Removing observers...\n";
    board->removeObserver(&observer1);
    board->removeObserver(&observer2);
    board->removeObserver(&observer3);
    std::cout << "Multiple observers test: PASS\n";
}

/**
 * @test Testing square access 
 */
void GameBoardTest::testInvalidSquareAccess() {
    GameBoard* board = getTestBoard();
    
    // Test out of bounds access
    try {
        board->getSquare(-1, 0);
        assert(false && "Should throw exception for negative row");
    } catch (const std::out_of_range&) {
        std::cout << "Invalid negative row access test: PASS\n";
    }
    
    try {
        board->getSquare(0, -1);
        assert(false && "Should throw exception for negative column");
    } catch (const std::out_of_range&) {
        std::cout << "Invalid negative column access test: PASS\n";
    }
    
    try {
        board->getSquare(GameBoard::BOARD_SIZE + 2, 0);
        assert(false && "Should throw exception for row > board size");
    } catch (const std::out_of_range&) {
        std::cout << "Invalid large row access test: PASS\n";
    }
}

/**
 * @test Testing invalid robot movement 
 */
void GameBoardTest::testInvalidRobotMovement() {
    GameBoard* board = getTestBoard();
    RobotMoveRequest request;
    
    try {
        for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
            for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
                if (board->getSquare(i, j).redRobotPresent()) {
                    board->getSquare(i, j).setRedRobot(false);
                }
                if (board->getSquare(i, j).blueRobotPresent()) {
                    board->getSquare(i, j).setBlueRobot(false);
                }
            }
        }
        
        request.robot = RobotMoveRequest::RED;
        request.move = RobotMoveRequest::FORWARD;
        board->moveRobot(request);
        assert(false && "Should throw exception for non-existent robot");
    } catch (const std::runtime_error& e) {
        std::string error = e.what();
        if (error == "Robot not found on board") {
            std::cout << "Non-existent robot movement test: PASS\n";
        } else {
            throw; 
        }
    }
    
    try {
        board->getSquare(8, 8).setRedRobot(true);
        request.move = static_cast<RobotMoveRequest::RobotMove>(999);
        board->moveRobot(request);
        assert(false && "Should throw exception for invalid move type");
    } catch (const std::invalid_argument& e) {
        std::cout << "Invalid move type test: PASS\n";
    }
}

/**
 * @test Testing behavior when there's an invalid painting request 
 */
void GameBoardTest::testInvalidPaintBlobRequest() {
    GameBoard* board = getTestBoard();
    RobotMoveRequest request;
    
    try {
        request.robot = RobotMoveRequest::RED;
        request.shoot = true;
        
        for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
            for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
                board->getSquare(i, j).setRedRobot(false);
                board->getSquare(i, j).setBlueRobot(false);
            }
        }
        
        board->paintBlobHit(request);
        assert(false && "Should throw exception for non-existent shooting robot");
    } catch (const std::runtime_error&) {
        std::cout << "Non-existent shooting robot test: PASS\n";
    }
}

// void GameBoardTest::testInvalidColorAssignment() {
//     GameBoard* board = getTestBoard();
    
//     try {
//         board->setSquareColor(1, 1, static_cast<SquareColor>(999));
//         assert(false && "Should throw exception for invalid color");
//     } catch (const std::invalid_argument&) {
//         std::cout << "Invalid color assignment test: PASS\n";
//     }
    
//     try {
//         board->setRobotPaintColor(RobotMoveRequest::RED, static_cast<SquareColor>(999));
//         assert(false && "Should throw exception for invalid robot paint color");
//     } catch (const std::invalid_argument&) {
//         std::cout << "Invalid robot paint color test: PASS\n";
//     }
// }

/**
 * @test Testing correct access of observers 
 */
void GameBoardTest::testInvalidObserverOperations() {
    GameBoard* board = getTestBoard();
    
    Observer* nonExistentObserver = new MockObserver();
    board->removeObserver(nonExistentObserver);
    std::cout << "Non-existent observer removal (should not crash) test: PASS\n";
    delete nonExistentObserver;
    
    try {
        board->addObserver(nullptr);
        assert(false && "Should throw exception for null observer");
    } catch (const std::invalid_argument&) {
        std::cout << "Null observer addition test: PASS\n";
    }
}

/**
 * @test Testing program behavior with invalid scan requests 
 */
void GameBoardTest::testInvalidScanRequests() {
    GameBoard* board = getTestBoard();
    
    try {
        board->getShortRangeScan(static_cast<RobotMoveRequest::RobotColor>(999));
        assert(false && "Should throw exception for invalid robot color in scan");
    } catch (const std::invalid_argument&) {
        std::cout << "Invalid scan robot color test: PASS\n";
    }
}

void GameBoardTest::testMemoryLeakPrevention() {
    GameBoard* board = getTestBoard();
    
    for (int i = 0; i < 100; ++i) {
        ExternalBoardSquare** scan = board->getLongRangeScan();
        for (int j = 0; j < GameBoard::BOARD_SIZE + 2; ++j) {
            delete[] scan[j];
        }
        delete[] scan;
    }
    std::cout << "Memory leak prevention test: PASS\n";
}

/**
 * @test Testing when two moves are made simultaneously 
 */
void GameBoardTest::testConcurrentModificationHandling() {
    GameBoard* board = getTestBoard();
    
    // Reset robot position if needed
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            if (board->getSquare(i, j).getSquareType() == EMPTY) {
                board->getSquare(i, j).setRedRobot(true);
                break;
            }
        }
    }
    
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.move = RobotMoveRequest::FORWARD;
    
    for (int i = 0; i < 100; ++i) {
        board->moveRobot(request);
        board->getLongRangeScan();  
    }
    std::cout << "Concurrent modification handling test: PASS\n";
}

/**
 * @test Testing when null pointer or empty string is passed in as password 
 */
void GameBoardTest::testNullPointerHandling() {
    try {
        GameBoard::getInstance("");
        assert(false && "Should throw exception for empty password");
    } catch (const std::runtime_error& e) {
        std::cout << "Empty password handling test: PASS\n";
    }
}

/**
 * @test Testing whether exceptions behave properly with observers 
 */
void GameBoardTest::testExceptionPropagation() {
    GameBoard* board = getTestBoard();
    
    class ThrowingObserver : public Observer {
        public:
            void update(Observable* subject) override {
                throw std::runtime_error("Test exception");
            }
    };
    
    ThrowingObserver throwingObserver;
    board->addObserver(&throwingObserver);
    
    try {
        RobotMoveRequest request;
        request.robot = RobotMoveRequest::RED;
        request.move = RobotMoveRequest::FORWARD;
        board->moveRobot(request);
        std::cout << "Exception propagation handling test: PASS\n";
    } catch (...) {
        board->removeObserver(&throwingObserver);
        assert(false && "Should handle observer exceptions gracefully");
    }
    
    board->removeObserver(&throwingObserver);
}


void GameBoardTest::testPaintBlobHitColorChange() {
    GameBoard* board = getTestBoard();
    
    // Reset board state
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
            board->getSquare(i, j).setSquareColor(WHITE);
            board->getSquare(i, j).setType(EMPTY);
        }
    }
    
    std::cout << "\nInitial Setup:" << std::endl;
    
    // Place robots in known positions
    board->getSquare(8, 8).setRedRobot(true);
    board->getSquare(8, 8).setRobotDirection(NORTH);
    board->getSquare(7, 8).setBlueRobot(true);
    
    std::cout << "Red robot placed at (8,8) facing NORTH" << std::endl;
    std::cout << "Blue robot placed at (7,8)" << std::endl;
    
    // Red robot shoots blue robot
    RobotMoveRequest shootRequest;
    shootRequest.robot = RobotMoveRequest::RED;
    shootRequest.shoot = true;
    shootRequest.move = RobotMoveRequest::NONE;
    
    // Verify hit is successful
    bool hitResult = board->paintBlobHit(shootRequest);
    std::cout << "Paint blob hit result: " << (hitResult ? "HIT" : "MISS") << std::endl;
    assert(hitResult);
    
    // Move blue robot and verify it paints red
    RobotMoveRequest moveRequest;
    moveRequest.robot = RobotMoveRequest::BLUE;
    moveRequest.move = RobotMoveRequest::FORWARD;
    moveRequest.shoot = false;
    
    bool moveResult = board->moveRobot(moveRequest);
    std::cout << "Blue robot move result: " << (moveResult ? "SUCCESS" : "FAIL") << std::endl;
    
    // Print blue robot's current paint color
    std::cout << "Blue robot paint color after hit: " << (board->getSquare(7, 8).getSquareColor()) << std::endl;
    
    // Get board state to verify colors
    ExternalBoardSquare** scan = board->getLongRangeScan();
    
    // Print the state of several squares around where we expect the color change
    std::cout << "\nBoard state after move:" << std::endl;
    for (int i = 5; i <= 9; ++i) {
        for (int j = 6; j <= 10; ++j) {
            std::cout << "Position (" << i << "," << j << "): "
                     << "Color=" << scan[i][j].getSquareColor()
                     << " BlueRobot=" << scan[i][j].blueRobotPresent()
                     << " RedRobot=" << scan[i][j].redRobotPresent()
                     << std::endl;
        }
    }
    
    // Check the specific square we're testing
    SquareColor squareColor = scan[6][8].getSquareColor();
    std::cout << "\nTarget square (6,8) color: " << squareColor 
              << " (Expecting " << RED << ")" << std::endl;
    
    // Clean up the scan memory
    for (int i = 0; i < GameBoard::BOARD_SIZE + 2; ++i) {
        delete[] scan[i];
    }
    delete[] scan;
    
    // Now do the assertion
    assert(squareColor == RED);
    
    std::cout << "Paint blob color change test: PASS\n";
}

void GameBoardTest::testPaintBlobHitDuration() {
    GameBoard* board = getTestBoard();
    Config& config = Config::getInstance();
    
    // Reset board state
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
            board->getSquare(i, j).setSquareColor(WHITE);
            board->getSquare(i, j).setType(EMPTY);
        }
    }
    
    // Setup robots - start them lower on the board to prevent going out of bounds
    board->getSquare(12, 8).setRedRobot(true);
    board->getSquare(12, 8).setRobotDirection(NORTH);
    board->getSquare(11, 8).setBlueRobot(true);
    
    std::cout << "Initial robot positions set" << std::endl;
    
    // Red hits blue
    RobotMoveRequest shootRequest;
    shootRequest.robot = RobotMoveRequest::RED;
    shootRequest.shoot = true;
    shootRequest.move = RobotMoveRequest::NONE;
    
    bool hitResult = board->paintBlobHit(shootRequest);
    std::cout << "Paint blob hit result: " << (hitResult ? "HIT" : "MISS") << std::endl;
    assert(hitResult);
    
    // Move blue robot config.getHitDuration() - 1 times
    RobotMoveRequest moveRequest;
    moveRequest.robot = RobotMoveRequest::BLUE;
    moveRequest.move = RobotMoveRequest::FORWARD;
    moveRequest.shoot = false;
    
    int moveCount = 0;
    int maxMoves = std::min(config.getHitDuration() - 1, 5);  // Limit moves to avoid board edges
    
    std::cout << "Starting moves, will make " << maxMoves << " moves" << std::endl;
    
    for (int i = 0; i < maxMoves; i++) {
        bool moveResult = board->moveRobot(moveRequest);
        if (!moveResult) {
            std::cout << "Move " << i << " failed" << std::endl;
            break;
        }
        moveCount++;
        
        // Get current board state
        ExternalBoardSquare** scan = board->getLongRangeScan();
        int currentRow = 11 - i;  // Starting from row 11 moving up
        SquareColor squareColor = scan[currentRow][8].getSquareColor();
        
        // Debug output
        std::cout << "Move " << i << " - Square (" << currentRow << ",8) color: " << squareColor << std::endl;
        
        // Clean up scan memory before assertion
        for (int j = 0; j < GameBoard::BOARD_SIZE + 2; ++j) {
            delete[] scan[j];
        }
        delete[] scan;
        
        // Should still be painting red
        assert(squareColor == RED);
    }
    
    std::cout << "Completed " << moveCount << " moves successfully" << std::endl;
    std::cout << "Paint blob hit duration test: PASS\n";
}

void GameBoardTest::testPaintBlobHitExpiration() {
    GameBoard* board = getTestBoard();
    Config& config = Config::getInstance();
    
    // Reset board state
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
            board->getSquare(i, j).setSquareColor(WHITE);
            board->getSquare(i, j).setType(EMPTY);
        }
    }
    
    std::cout << "\nStarting paint blob hit expiration test" << std::endl;
    
    // Setup robots - start at bottom of board to allow more moves upward
    int startRow = GameBoard::BOARD_SIZE - 1;  // row 14
    board->getSquare(startRow, 8).setRedRobot(true);
    board->getSquare(startRow, 8).setRobotDirection(NORTH);
    board->getSquare(startRow - 1, 8).setBlueRobot(true);
    
    std::cout << "Placed red robot at (" << startRow << ",8)" << std::endl;
    std::cout << "Placed blue robot at (" << (startRow-1) << ",8)" << std::endl;
    
    // Red hits blue
    RobotMoveRequest shootRequest;
    shootRequest.robot = RobotMoveRequest::RED;
    shootRequest.shoot = true;
    shootRequest.move = RobotMoveRequest::NONE;
    
    bool hitResult = board->paintBlobHit(shootRequest);
    assert(hitResult);
    std::cout << "Paint blob hit successful" << std::endl;
    
    // Move blue robot for hit duration
    RobotMoveRequest moveRequest;
    moveRequest.robot = RobotMoveRequest::BLUE;
    moveRequest.move = RobotMoveRequest::FORWARD;
    moveRequest.shoot = false;
    
    int successfulMoves = 0;
    
    // Make moves during hit duration
    std::cout << "Moving robot during hit duration (" << config.getHitDuration() << " moves)" << std::endl;
    for (int i = 0; i < config.getHitDuration(); i++) {
        if (!board->moveRobot(moveRequest)) {
            std::cout << "Move " << i << " failed - hit board boundary" << std::endl;
            break;
        }
        successfulMoves++;
        
        // Verify painting red during hit duration
        ExternalBoardSquare** scan = board->getLongRangeScan();
        SquareColor color = scan[startRow - 2 - i][8].getSquareColor();
        
        // Cleanup scan
        for (int j = 0; j < GameBoard::BOARD_SIZE + 2; ++j) {
            delete[] scan[j];
        }
        delete[] scan;
        
        assert(color == RED);
    }
    
    std::cout << "Completed " << successfulMoves << " moves with red paint" << std::endl;
    
    // Make one more move after hit duration (if we haven't hit the board boundary)
    if (successfulMoves == config.getHitDuration() && 
        startRow - 2 - config.getHitDuration() > 1) {  // Check if next move is valid
        
        bool finalMove = board->moveRobot(moveRequest);
        assert(finalMove);
        
        // Get the color of the last moved square
        ExternalBoardSquare** scan = board->getLongRangeScan();
        SquareColor finalColor = scan[startRow - 2 - config.getHitDuration()][8].getSquareColor();
        
        // Cleanup scan
        for (int i = 0; i < GameBoard::BOARD_SIZE + 2; ++i) {
            delete[] scan[i];
        }
        delete[] scan;
        
        // Verify it's now painting blue
        assert(finalColor == BLUE);
        std::cout << "Final move painted BLUE as expected" << std::endl;
    } else {
        std::cout << "Skipped final move verification due to board boundary" << std::endl;
    }
    
    std::cout << "Paint blob hit expiration test: PASS\n";
}

void GameBoardTest::testMultipleRobotHits() {
    GameBoard* board = getTestBoard();
    
    // Reset board state
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
            board->getSquare(i, j).setSquareColor(WHITE);
            board->getSquare(i, j).setType(EMPTY);
        }
    }
    
    std::cout << "\nStarting multiple robot hits test" << std::endl;
    
    // Place robots with clear lines of sight for shooting
    board->getSquare(8, 6).setRedRobot(true);
    board->getSquare(8, 6).setRobotDirection(EAST);   // Red faces east
    board->getSquare(8, 8).setBlueRobot(true);       // Blue is directly east of red
    board->getSquare(8, 8).setRobotDirection(WEST);  // Blue faces west (toward red)
    
    std::cout << "Initial setup:\n";
    std::cout << "Red robot at (8,6) facing EAST\n";
    std::cout << "Blue robot at (8,8) facing WEST\n";
    
    // Red hits blue
    RobotMoveRequest redShoot;
    redShoot.robot = RobotMoveRequest::RED;
    redShoot.shoot = true;
    redShoot.move = RobotMoveRequest::NONE;
    
    bool redHit = board->paintBlobHit(redShoot);
    std::cout << "Red robot paint blob hit: " << (redHit ? "SUCCESS" : "FAIL") << std::endl;
    if (!redHit) {
        std::cout << "Red robot couldn't hit blue robot - check line of sight\n";
    }
    assert(redHit);
    
    // Blue hits red
    RobotMoveRequest blueShoot;
    blueShoot.robot = RobotMoveRequest::BLUE;
    blueShoot.shoot = true;
    blueShoot.move = RobotMoveRequest::NONE;
    
    bool blueHit = board->paintBlobHit(blueShoot);
    std::cout << "Blue robot paint blob hit: " << (blueHit ? "SUCCESS" : "FAIL") << std::endl;
    assert(blueHit);
    
    // After hits, rotate blue robot north so it can move without collision
    RobotMoveRequest blueRotate;
    blueRotate.robot = RobotMoveRequest::BLUE;
    blueRotate.move = RobotMoveRequest::ROTATELEFT;
    blueRotate.shoot = false;
    board->moveRobot(blueRotate);
    
    // Move both robots
    RobotMoveRequest redMove;
    redMove.robot = RobotMoveRequest::RED;
    redMove.move = RobotMoveRequest::FORWARD;
    redMove.shoot = false;
    
    RobotMoveRequest blueMove;
    blueMove.robot = RobotMoveRequest::BLUE;
    blueMove.move = RobotMoveRequest::FORWARD;
    blueMove.shoot = false;
    
    bool redMoved = board->moveRobot(redMove);
    std::cout << "Red robot move: " << (redMoved ? "SUCCESS" : "FAIL") << std::endl;
    assert(redMoved);
    
    bool blueMoved = board->moveRobot(blueMove);
    std::cout << "Blue robot move: " << (blueMoved ? "SUCCESS" : "FAIL") << std::endl;
    assert(blueMoved);
    
    // Get final board state
    ExternalBoardSquare** scan = board->getLongRangeScan();
    
    // Check original positions for correct colors
    SquareColor redSquareColor = scan[8][6].getSquareColor();
    SquareColor blueSquareColor = scan[8][8].getSquareColor();
    
    std::cout << "Original red square (8,6) color: " << redSquareColor << " (Expecting BLUE)" << std::endl;
    std::cout << "Original blue square (8,8) color: " << blueSquareColor << " (Expecting RED)" << std::endl;
    
    // Cleanup scan
    for (int i = 0; i < GameBoard::BOARD_SIZE + 2; ++i) {
        delete[] scan[i];
    }
    delete[] scan;
    
    // Verify both robots painted with switched colors
    assert(redSquareColor == BLUE);
    assert(blueSquareColor == RED);
    
    std::cout << "Multiple robot hits test: PASS\n";
}


void GameBoardTest::testPaintBlobLimit() {
    GameBoard* board = getTestBoard();
    
    // Reset board state
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
            board->getSquare(i, j).setType(EMPTY);
        }
    }
    
    // Place robots in position for repeated shots
    board->getSquare(8, 6).setRedRobot(true);
    board->getSquare(8, 6).setRobotDirection(EAST);
    board->getSquare(8, 8).setBlueRobot(true);
    
    RobotMoveRequest shootRequest;
    shootRequest.robot = RobotMoveRequest::RED;
    shootRequest.shoot = true;
    shootRequest.move = RobotMoveRequest::NONE;
    
    int initialPaintBlobs = board->getRemainingPaintBlobs(RobotMoveRequest::RED);
    std::cout << "Initial paint blobs: " << initialPaintBlobs << std::endl;
    
    // Try to shoot more than the limit
    int successfulShots = 0;
    for (int i = 0; i < initialPaintBlobs + 5; i++) {
        if (board->paintBlobHit(shootRequest)) {
            successfulShots++;
        }
    }
    
    std::cout << "Successful shots: " << successfulShots << std::endl;
    assert(successfulShots == initialPaintBlobs);
    assert(board->getRemainingPaintBlobs(RobotMoveRequest::RED) == 0);
    
    // Verify no more shots are possible
    assert(!board->paintBlobHit(shootRequest));
    
    std::cout << "Paint blob limit test: PASS\n";
}

// void GameBoardTest::testPaintBlobLimitReset() {
//     GameBoard* board = getTestBoard();
//     Config& config = Config::getInstance();
    
//     std::cout << "Initial paint blob counts - Red: " 
//               << board->getRemainingPaintBlobs(RobotMoveRequest::RED)
//               << ", Expected: " << config.getPaintBlobLimit() << std::endl;
              
//     // Verify initial paint blob counts
//     assert(board->getRemainingPaintBlobs(RobotMoveRequest::RED) == config.getPaintBlobLimit());
//     assert(board->getRemainingPaintBlobs(RobotMoveRequest::BLUE) == config.getPaintBlobLimit());
    
//     std::cout << "Paint blob limit reset test: PASS\n";
// }



bool GameBoardTest::doTests() {
    bool allTestsPassed = true;
    try {
        std::cout << "-------------------------\n";
        std::cout << "GameBoard class unit tests\n";
        std::cout << "-------------------------\n";
        
        // 1. Singleton Pattern Tests
        setUp(); 
        testGetInstanceWithPassword();
        testGetInstanceWithWrongPassword();
        testSingletonBehavior();
        testPasswordProtection();
        tearDown(); 

        // 2. Testing for correct board initialization 
        setUp(); 
        testBoardDimensions();
        testWallPlacement();
        testRockPlacement();
        testFogPlacement();
        testRobotInitialPlacement();
        testRobotPlacementValidation();
        tearDown(); 

        // 3. Tests for robot movement 
        setUp(); 
        testBasicRobotMovements();
        testWallCollisionPrevention();
        testRockCollisionPrevention();
        testFogMovement();
        testBoardBoundaryPrevention();
        testColorTrails();
        testRobotCollisionPrevention();
        tearDown(); 

        // 4. Paint Blob Tests
        setUp();
        testPaintBlobHit();
        testPaintBlobMiss();
        testPaintBlobRockBlocking();
        testPaintBlobFogBehavior();
        testPaintBlobWallCollision();
        testPaintBlobRange();
        testPaintBlobDirection();
        tearDown();

        // 5. Score Tracking Tests
        setUp();
        testInitialScore();
        testScoreIncrementOnMovement();
        testScoreUpdateOnPaintBlob();
        testScoreAfterMultipleOperations();
        tearDown(); 

        // 6. Short Range Scan Tests
        setUp();
        testShortRangeScanBasic();
        testShortRangeScanRobotPosition();
        testShortRangeScanDirections();
        testShortRangeScanWallBoundaries();
        testShortRangeScanFogBehavior();
        testShortRangeScanObstructions();
        tearDown();

        // 7. Long range scan tests: 
        setUp();
        testLongRangeDimensions();
        testLongRangeWalls();
        testLongRangeRocks();
        testLongRangeFog();
        testLongRangeRobots();
        testLongRangeColors();
        tearDown();

        // 8. Test observer pattern tests
        setUp();
        testMovementNotification();
        testColorChangeNotification();
        testScoreChangeNotification();
        testObserverManagement();
        testMultipleObservers();
        tearDown();

        // Error handling tests
        setUp();
        testInvalidSquareAccess();
        testInvalidRobotMovement();
        testInvalidPaintBlobRequest();
        // testInvalidColorAssignment();
        testInvalidObserverOperations();
        testInvalidScanRequests();
        testMemoryLeakPrevention();
        testConcurrentModificationHandling();
        testNullPointerHandling();
        testExceptionPropagation();
        tearDown();

        setUp(); 
        testPaintBlobHitColorChange();
        testPaintBlobHitDuration();
        testPaintBlobHitExpiration();
        testMultipleRobotHits();
        tearDown(); 

        setUp(); 
        testPaintBlobLimit();
        // testPaintBlobLimitReset();

        
   
    } catch (const std::exception& e) {
        std::cout << "Test failed with exception: " << e.what() << std::endl;
        allTestsPassed = false;
    }
    
    return allTestsPassed;
}

} 

int main() {
    Paintbots::GameBoardTest tester;
    return tester.doTests() ? 0 : 1;
}