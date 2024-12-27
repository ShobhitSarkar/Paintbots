#include "PlainDisplayTest.h"
#include "PlainDisplay.h"
#include <iostream>
#include <sstream>  
#include <cassert>
#include <cstring>  

#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {

// OutputCapture implementation
PlainDisplayTest::OutputCapture::OutputCapture() : 
    capture(),                                   
    oldCout(std::cout.rdbuf(capture.rdbuf())) 
{}

PlainDisplayTest::OutputCapture::~OutputCapture() { 
    std::cout.rdbuf(oldCout); 
}

std::string PlainDisplayTest::OutputCapture::getOutput() { 
    return capture.str(); 
}

// Helper methods implementation
GameBoard* PlainDisplayTest::getTestBoard() {
    return GameBoard::getInstance("xyzzy");
}

void PlainDisplayTest::setUp() {
    GameBoard::resetInstance();
}

void PlainDisplayTest::tearDown() {
    GameBoard::resetInstance();
}

// Test methods implementation
void PlainDisplayTest::testInitialization() {
    PlainDisplay display;
    OutputCapture capture;
    display.display();
    std::string output = capture.getOutput();
    
    // Verify board dimensions
    size_t newlines = std::count(output.begin(), output.end(), '\n');
    
    // Print debug info BEFORE assertion
    std::cerr << "Debug Output:" << std::endl;
    std::cerr << "Found " << newlines << " newlines" << std::endl;
    std::cerr << "Expected 18 newlines" << std::endl;
    std::cerr << "Actual output:" << std::endl;
    std::cerr << output << std::endl;
    std::cerr << "End of output" << std::endl;
    
    // Now do the assertion
    if (newlines != 18) {
        std::cerr << "Test failed: Expected 18 newlines, got " << newlines << std::endl;
    }
    assert(newlines == 18); // 17 rows + 1 header row
    
    // Verify initial state (all white squares)
    assert(output.find("W- ") != std::string::npos);
    std::cout << "Initialization test: PASS\n";
}

void PlainDisplayTest::testWallDisplay() {
    GameBoard* board = getTestBoard();
    PlainDisplay display;
    board->addObserver(&display);
    board->notifyObservers();
    
    OutputCapture capture;
    display.display(); 
    std::string output = capture.getOutput();
    
    assert(output.find("W-X") != std::string::npos);
    
    board->removeObserver(&display);
    std::cout << "Wall display test: PASS\n";
}

void PlainDisplayTest::testRobotDisplay() {
    GameBoard* board = getTestBoard();
    PlainDisplay display;
    board->addObserver(&display);

    // Place robots in known positions
    board->getSquare(8, 8).setRedRobot(true);
    board->getSquare(10, 10).setBlueRobot(true);

    // Add this line to notify the display about the robots
    board->notifyObservers();

    OutputCapture capture;
    display.display(); 
    std::string output = capture.getOutput();

    // Check for robot representations
    assert(output.find("RR") != std::string::npos); // Red robot
    assert(output.find("BR") != std::string::npos); // Blue robot

    board->removeObserver(&display);
    std::cout << "Robot display test: PASS\n";
}

void PlainDisplayTest::testRockDisplay() {
    GameBoard* board = getTestBoard();
    PlainDisplay display;
    board->addObserver(&display);

    // Place a rock
    board->getSquare(5, 5).setType(ROCK);

    board->notifyObservers();

    OutputCapture capture;
    display.display(); 
    std::string output = capture.getOutput();

    // Debug: Print entire output
    std::cerr << "Complete Output:\n" << output << "\n";

    // Check for rock representation
    bool rockFound = output.find("W-R") != std::string::npos;
    if (!rockFound) {
        std::cerr << "Rock representation not found! Checking for other variations...\n";
        if (output.find("R") != std::string::npos) {
            std::cerr << "R was found in output\n";
        }
    }

    assert(rockFound);

    board->removeObserver(&display);
    std::cout << "Rock display test: PASS\n";
}

void PlainDisplayTest::testFogDisplay() {
    GameBoard* board = getTestBoard();
    PlainDisplay display;
    board->addObserver(&display);

    // Place fog
    board->getSquare(7, 7).setType(FOG);

    board->notifyObservers();

    OutputCapture capture;
    display.display(); 
    std::string output = capture.getOutput();

    // Check for fog representation
    assert(output.find("W-F") != std::string::npos);

    board->removeObserver(&display);
    std::cout << "Fog display test: PASS\n";
}

void PlainDisplayTest::testColorDisplay() {
    GameBoard* board = getTestBoard();
    PlainDisplay display;
    board->addObserver(&display);

    // Debugging: Track robot movement and color changes
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.move = RobotMoveRequest::FORWARD;

    // Debug print initial state
    std::cout << "Initial Robot Position:\n";
    for (int row = 1; row <= GameBoard::BOARD_SIZE; ++row) {
        for (int col = 1; col <= GameBoard::BOARD_SIZE; ++col) {
            if (board->getSquare(row, col).redRobotPresent()) {
                std::cout << "Red Robot at (" << row << "," << col 
                          << ") facing " << board->getSquare(row, col).robotDirection() << std::endl;
            }
        }
    }

    // Manually set colors
    board->setSquareColor(5, 5, RED);
    board->setSquareColor(6, 6, BLUE);
    board->setSquareColor(7, 7, WHITE);

    // Move robot to trigger potential color mechanisms
    board->moveRobot(request);

    OutputCapture capture;
    display.display();
    std::string output = capture.getOutput();

    // Detailed color debugging
    std::cout << "Detailed Color Check:\n";
    for (int row = 1; row <= GameBoard::BOARD_SIZE; ++row) {
        for (int col = 1; col <= GameBoard::BOARD_SIZE; ++col) {
            SquareColor color = board->getSquare(row, col).getSquareColor();
            if (color != WHITE) {
                std::cout << "Square (" << row << "," << col 
                          << ") color: " 
                          << (color == RED ? "RED" : 
                              (color == BLUE ? "BLUE" : "WHITE")) 
                          << std::endl;
            }
        }
    }

    // Debug: Full board output
    std::cout << "Full Board Output:\n" << output << "\n";

    // Check for color prefixes
    assert(output.find("R-") != std::string::npos); // Red
    assert(output.find("B-") != std::string::npos); // Blue
    assert(output.find("W-") != std::string::npos); // White

    board->removeObserver(&display);
    std::cout << "Color display test: PASS\n";
}

void PlainDisplayTest::testObserverUpdate() {
    GameBoard* board = getTestBoard();
    PlainDisplay display;
    board->addObserver(&display);

    // Make a move that should trigger an update
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.move = RobotMoveRequest::FORWARD;

    OutputCapture capture1;
    display.display();
    std::string before = capture1.getOutput();

    board->moveRobot(request);

    OutputCapture capture2;
    display.display();
    std::string after = capture2.getOutput();

    // Verify display changed after move
    assert(before != after);

    board->removeObserver(&display);
    std::cout << "Observer update test: PASS\n";
}

void PlainDisplayTest::testCoordinateDisplay() {
    PlainDisplay display;
    OutputCapture capture;
    display.display();
    std::string output = capture.getOutput();

    // Check for row numbers
    for (int i = 0; i <= 16; i++) {
        assert(output.find(std::to_string(i)) != std::string::npos);
    }

    std::cout << "Coordinate display test: PASS\n";
}

void PlainDisplayTest::testMultipleUpdates() {
    GameBoard* board = getTestBoard();
    PlainDisplay display;
    board->addObserver(&display);

    // Perform multiple moves
    RobotMoveRequest request;
    request.robot = RobotMoveRequest::RED;
    request.move = RobotMoveRequest::FORWARD;

    // Reset board to ensure a known starting state
    for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
        for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
            board->getSquare(i, j).setRedRobot(false);
            board->getSquare(i, j).setBlueRobot(false);
            board->getSquare(i, j).setType(EMPTY);
            board->getSquare(i, j).setSquareColor(WHITE);
        }
    }

    // Place red robot in a known position
    board->getSquare(8, 8).setRedRobot(true);
    board->getSquare(8, 8).setRobotDirection(NORTH);

    std::string lastOutput;
    for (int i = 0; i < 3; i++) {
        std::cout << "Move iteration: " << i << std::endl;
        
        // Print board state before display
        std::cout << "Board state before display:" << std::endl;
        for (int row = 1; row <= GameBoard::BOARD_SIZE; ++row) {
            for (int col = 1; col <= GameBoard::BOARD_SIZE; ++col) {
                if (board->getSquare(row, col).redRobotPresent()) {
                    std::cout << "Red robot at (" << row << ", " << col 
                              << ") facing " << board->getSquare(row, col).robotDirection() << std::endl;
                }
            }
        }
        
        OutputCapture capture;
        display.display();
        std::string currentOutput = capture.getOutput();
        
        std::cout << "Current output:" << std::endl;
        std::cout << currentOutput << std::endl;
        
        if (i > 0) {
            std::cout << "Comparing current output with last output..." << std::endl;
            
            // Debug: print the differences
            for (size_t j = 0; j < std::min(lastOutput.length(), currentOutput.length()); ++j) {
                if (lastOutput[j] != currentOutput[j]) {
                    std::cout << "First difference at index " << j 
                              << ": Last output: " << lastOutput[j] 
                              << ", Current output: " << currentOutput[j] << std::endl;
                    break;
                }
            }
            
            assert(currentOutput != lastOutput);
        }
        
        lastOutput = currentOutput;
        
        std::cout << "Performing robot move..." << std::endl;
        
        // Ensure the move actually changes something
        if (board->moveRobot(request)) {
            std::cout << "Move successful" << std::endl;
        } else {
            std::cout << "Move failed" << std::endl;
            break;
        }
    }

    board->removeObserver(&display);
    std::cout << "Multiple updates test: PASS\n";
}

void PlainDisplayTest::testComplexBoardState() {
    GameBoard* board = getTestBoard();
    PlainDisplay display;
    board->addObserver(&display);

    // Create a complex board state
    board->getSquare(5, 5).setType(ROCK);
    board->getSquare(6, 6).setType(FOG);
    board->getSquare(7, 7).setRedRobot(true);
    board->getSquare(8, 8).setBlueRobot(true);
    board->setSquareColor(9, 9, RED);
    board->setSquareColor(10, 10, BLUE);

    board->notifyObservers();

    OutputCapture capture;
    display.display();
    std::string output = capture.getOutput();

    // Debug: print the entire output
    std::cout << "Full output:\n" << output << "\n";

    // Verify rock representation with more detailed debugging
    bool rockFound = output.find("W-R") != std::string::npos;
    if (!rockFound) {
        std::cout << "Rock representation not found!\n";
        
        // Additional debugging for rock square
        ExternalBoardSquare** scan = board->getLongRangeScan();
        std::cout << "Rock square type from scan: " 
                  << scan[5][5].getSquareType() << std::endl;
        
        // Cleanup
        for (int i = 0; i < 17; ++i) {
            delete[] scan[i];
        }
        delete[] scan;
    }

    // Verify all elements are present
    assert(rockFound); // Rock
    assert(output.find("W-F") != std::string::npos); // Fog
    assert(output.find("RR") != std::string::npos);  // Red robot
    assert(output.find("BR") != std::string::npos);  // Blue robot
    assert(output.find("R-") != std::string::npos);  // Red square
    assert(output.find("B-") != std::string::npos);  // Blue square

    board->removeObserver(&display);
    std::cout << "Complex board state test: PASS\n";
}

void PlainDisplayTest::testMemoryManagement() {
    GameBoard* board = getTestBoard();
    PlainDisplay* display = new PlainDisplay();
    board->addObserver(display);

    // Perform multiple updates
    for (int i = 0; i < 100; i++) {
        display->display();
        RobotMoveRequest request;
        request.robot = RobotMoveRequest::RED;
        request.move = RobotMoveRequest::FORWARD;
        board->moveRobot(request);
    }

    board->removeObserver(display);
    delete display;
    std::cout << "Memory management test: PASS\n";
}

bool PlainDisplayTest::doTests() {
    bool allTestsPassed = true;
    try {
        std::cout << "-------------------------\n";
        std::cout << "PlainDisplay class unit tests\n";
        std::cout << "-------------------------\n";
        
        setUp();
        testInitialization();
        testWallDisplay();
        testRobotDisplay();
        testRockDisplay();
        testFogDisplay();
        testColorDisplay();
        testObserverUpdate();
        testCoordinateDisplay();
        testMultipleUpdates();
        testComplexBoardState();
        testMemoryManagement();
        tearDown();
        
    } catch (const std::exception& e) {
        std::cout << "Test failed with exception: " << e.what() << std::endl;
        allTestsPassed = false;
    }
    
    return allTestsPassed;
}

} // namespace Paintbots

int main() {
    Paintbots::PlainDisplayTest tester;
    return tester.doTests() ? 0 : 1;
}