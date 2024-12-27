#include "InternalBoardSquareTest.h"
#include "BoardSquare.h"
#include "InternalBoardSquare.h"
#include <iostream>
#include <type_traits>
#include <cassert>
#include <cstring>  

#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {

    /**
     * @test Testing whether InternalBoardSquare correctly inherits from ITest
     */
    void InternalBoardSquareTest::testInheritance() {
        bool isSubClass = std::is_base_of<BoardSquare, InternalBoardSquare>::value; 
        assert(isSubClass); 
        std::cout << "InternalBoardSquare inheritace test: PASS\n"; 
    }

    bool InternalBoardSquareTest::doTests(){
        bool allTestsPassed = true; 

        try{
            std::cout << "-------------------------------------\n"; 
            std::cout << "InternalBoardSquare class unit tests:\n"; 
            std::cout << "-------------------------------------\n"; 

            testInheritance(); 
        } catch (const std::exception& e){
            std::cout << "test failed with exception: " << e.what() << std::endl; 
            allTestsPassed = false; 
        }

        return allTestsPassed; 
    }
}

int main() {
    Paintbots::InternalBoardSquareTest tester;
    return tester.doTests() ? 0 : 1;
}