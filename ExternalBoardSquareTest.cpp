#include "ExternalBoardSquareTest.h"
#include "BoardSquare.h"
#include "ExternalBoardSquare.h"
#include <iostream>
#include <type_traits>
#include <cassert>
#include <cstring>  
#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {

    /**
     * @test Testing inheritance of ExternalBoardSquare from BoardSquare 
     */
    void ExternalBoardSquareTest::testInheritance() {
        bool isSubclass = std::is_base_of<BoardSquare, ExternalBoardSquare>::value;
        assert(isSubclass);
        std::cout << "ExternalBoardSquare inheritance test: PASS\n";
    }

    /**
     * Driver code for running the test 
     */
    bool ExternalBoardSquareTest::doTests() {
        bool allTestsPassed = true;
        try {
            std::cout << "--------------------------------\n";
            std::cout << "ExternalBoardSquare class unit tests\n";
            std::cout << "--------------------------------\n";
            
            testInheritance();
            
        } catch (const std::exception& e) {
            std::cout << "Test failed with exception: " << e.what() << std::endl;
            allTestsPassed = false;
        }
        
        return allTestsPassed;
    }

} 

int main() {
    Paintbots::ExternalBoardSquareTest tester;
    return tester.doTests() ? 0 : 1;
}