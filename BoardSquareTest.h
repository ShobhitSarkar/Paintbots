#ifndef BOARD_SQUARE_TEST_H 
#define BOARD_SQUARE_TEST_H 

#include "ITest.h"
#include <string> 
#include <cstring>  
#include <stdexcept>  
#include <algorithm> 

namespace Paintbots{
    
    class BoardSquareTest : public ITest{
        
        private: 
            /**
             * Tests default constructor 
             */
            void testDefaultConstructor(); 

            /**
             * Tests specific constructor 
             */
            void testSpecificConstructor(); 

            /**
             * Tests copy constructor 
             */
            void testCopyConstructor(); 

            /**
             * Tests assignment operator 
             */
            void testAssignmentOperator(); 

            /**
             * Tests whether getter returns the correct square color 
             */
            void testGetSquareColor(); 

            /**
             * Tests whether getter returns correct square type 
             */
            void testGetSquareType(); 

            /**
             * Tests whether getter returns if a red robot is present 
             */
            void testRedRobotPresent(); 

            /**
             * Tests whether getter returns if a blue robot is present 
             */
            void testBlueRobotPresent(); 

            /**
             * Tests whether getter returns a correct robot direction 
             */
            void testRobotDirection(); 
        
        public: 

            /**
             * Running the tests 
             */
            bool doTests() override; 
    }; 
}

#endif 