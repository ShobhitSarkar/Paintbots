#ifndef INTERNAL_BOARD_SQUARE_TEST_H 
#define INTERNAL_BOARD_SQUARE_TEST_H 

#include "ITest.h"
#include <cstring>  

#include <stdexcept>  
#include <algorithm> 

namespace Paintbots{
    
    class InternalBoardSquareTest : public ITest{
        private: 
            /**
             * Test to check whether class correctly inherits from ITest 
             */
            void testInheritance(); 

        public: 
            bool doTests() override; 
    }; 
}

#endif 