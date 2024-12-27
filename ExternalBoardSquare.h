#ifndef EXTERNAL_BOARD_SQUARE_H 
#define EXTERNAL_BOARD_SQUARE_H 

#include "BoardSquare.h"
#include "InternalBoardSquare.h"
#include <cstring>  
#include <stdexcept>  
#include <algorithm> 

namespace Paintbots{
    class ExternalBoardSquare : public BoardSquare {
        public: 
            /**
             * Default Constructor
             */
            ExternalBoardSquare() : BoardSquare() {}

            /**
             * Constructor an ExternalBoardSquare from an InternalSquareBoard object 
             */
            explicit ExternalBoardSquare(const InternalBoardSquare& square); 
    }; 
}

#endif