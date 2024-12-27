#ifndef EXTERNAL_BOARD_SQUARE_TEST_H
#define EXTERNAL_BOARD_SQUARE_TEST_H
#include <cstring>  
#include <stdexcept>  
#include <algorithm> 

#include "ITest.h"

namespace Paintbots {
    class ExternalBoardSquareTest : public ITest {
    private:
        /**
         * Test to make sure ExternalBoardSquare is inheriting from BoardSquare 
         */
        void testInheritance();

    public:
        bool doTests() override;
    };
}

#endif