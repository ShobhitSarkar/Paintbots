#include "PlainDisplay.h"
#include <iostream>
#include <iomanip>
#include <cstring> 

#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {

/**
 * Default constructor for PlainDisplay 
 */
PlainDisplay::PlainDisplay() {
    initializeBoard();
}

/**
 * Initializes  the display board with default values 
 */
void PlainDisplay::initializeBoard() {
    displayBoard.resize(BOARD_SIZE + 2);
    for (auto& row : displayBoard) {
        row.resize(BOARD_SIZE + 2);
        for (auto& square : row) {
            square = "W---";
        }
    }
}

/**
 * The number to format 
 */
std::string PlainDisplay::padNumber(int num) {
    return (num < 10 ? " " : "") + std::to_string(num);
}

/**
 * Generates the visual representation of a square based on its attributes 
 * (such as color, robot presence and special features)
 */
std::string PlainDisplay::getSquareRepresentation(const ExternalBoardSquare& square) {
    std::string repr;
    
    // First character: Board color (R/B/W)
    switch(square.getSquareColor()) {
        case RED: repr = "R"; break;
        case BLUE: repr = "B"; break;
        case WHITE: 
        default: repr = "W"; break;
    }
    
    // Second character: Robot presence (R/B/-)
    if (square.redRobotPresent()) {
        repr += "R";
    } else if (square.blueRobotPresent()) {
        repr += "B";
    } else {
        repr += "-";
    }
    
    // Third character: Rock presence (X/-)
    if (square.getSquareType() == ROCK) {
        repr += "X";
    } else {
        repr += "-";
    }
    
    // Fourth character: Fog presence (X/-)
    if (square.getSquareType() == FOG) {
        repr += "X";
    } else {
        repr += "-";
    }
    
    // Override for walls
    if (square.getSquareType() == WALL) {
        return "WWWW";
    }
    
    return repr;
}

/**
 * Retrieves the latest state of the game board and updates the internal 
 * representation of the display board to reflect the changes 
 */
void PlainDisplay::updateBoardState(GameBoard* board) {
    if (!board) return;
    
    ExternalBoardSquare** scan = board->getLongRangeScan();
    
    for (int i = 0; i < BOARD_SIZE + 2; i++) {
        for (int j = 0; j < BOARD_SIZE + 2; j++) {
            displayBoard[i][j] = getSquareRepresentation(scan[i][j]);
        }
    }
    
    // Cleanup scan memory
    for (int i = 0; i < BOARD_SIZE + 2; i++) {
        delete[] scan[i];
    }
    delete[] scan;
}

/**
 * Updates the PlainDisplay object when the observed GameBoard notifies its users
 */
void PlainDisplay::update(Observable* subject) {
    GameBoard* board = static_cast<GameBoard*>(subject);
    if (board) {
        updateBoardState(board);
    }
}

/**
 * Renders the game board to the console 
 */
void PlainDisplay::display() {
    // Print column numbers
    std::cout << "   ";
    for (int j = 0; j < BOARD_SIZE + 2; j++) {
        std::cout << padNumber(j) << " ";
    }
    std::cout << "\n";
    
    // Print board with row numbers
    for (int i = 0; i < BOARD_SIZE + 2; i++) {
        std::cout << padNumber(i) << " ";
        
        for (int j = 0; j < BOARD_SIZE + 2; j++) {
            std::cout << displayBoard[i][j] << " ";
        }
        std::cout << "\n";
    }
}

}