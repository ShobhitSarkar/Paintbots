#ifndef PLAIN_DISPLAY_H 
#define PLAIN_DISPLAY_H 

#include "ObserverPattern.h"
#include "GameBoard.h"
#include "BoardSquare.h"
#include <vector> 
#include <string> 
#include <cstring> 

#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {
    class PlainDisplay : public Observer {
        private: 
            static const int BOARD_SIZE = 15; // constant to set the board size 
            std::vector<std::vector<std::string>> displayBoard; // vector of strings to print out the board 

            /**
             * Fills the display board with placeholder values to represent empty, white squares 
             */
            void initializeBoard();

            /**
             * Determines visual representation of a square based on its attributes 
             */
            std::string getSquareRepresentation(const ExternalBoardSquare& square);

            /**
             * Retrieves the latest state of the game board and updates the internal 
             * representation of the display board
             */
            void updateBoardState(GameBoard* board); 

            /**
             * Ensures single digit numbers are padded with space for proper allignment 
             */
            std::string padNumber(int num);

        public: 
            /**
             * Constructor 
             */
            PlainDisplay();

            /**
             * Updates PlainDisplay object when observed "GameBoard" notifies it's observers
             */
            void update(Observable* subject) override; 

            /**
             * Displays current state of the game board
             */
            void display(); 
    }; 
}

#endif 