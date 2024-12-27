#include "BoardSquareTest.h"
#include "BoardSquare.h"
#include <iostream>
#include <cassert>
#include <cstring> 
#include <stdexcept>  
#include <algorithm>  

namespace Paintbots{

    /**
     * @test Tests whether the default constructor works properly 
     */
    void BoardSquareTest::testDefaultConstructor() {
        BoardSquare square;
        assert(square.getSquareColor() == WHITE);
        assert(square.getSquareType() == EMPTY);
        assert(!square.redRobotPresent());
        assert(!square.blueRobotPresent());
        assert(square.robotDirection() == NORTH);
        std::cout << "Default constructor test: PASS\n";
    }

    /**
     * @test Tests the specific constructor 
     */
    void BoardSquareTest::testSpecificConstructor() {
        BoardSquare square(RED, true, false, SOUTH, ROCK);
        assert(square.getSquareColor() == RED);
        assert(square.redRobotPresent() == true);
        assert(square.blueRobotPresent() == false);
        assert(square.robotDirection() == SOUTH);
        assert(square.getSquareType() == ROCK);
        std::cout << "Specific constructor test: PASS\n";
    }

    /**
     * @test Tests the copy constructor 
     */
    void BoardSquareTest::testCopyConstructor() {
        BoardSquare original(BLUE, false, true, EAST, FOG);
        BoardSquare copy(original);
        
        assert(copy.getSquareColor() == BLUE);
        assert(copy.redRobotPresent() == false);
        assert(copy.blueRobotPresent() == true);
        assert(copy.robotDirection() == EAST);
        assert(copy.getSquareType() == FOG);
        std::cout << "Copy constructor test: PASS\n";
    }

    /**
     * @test Tests the assignment operator 
     */
    void BoardSquareTest::testAssignmentOperator() {
        BoardSquare square1(RED, true, false, WEST, ROCK);
        BoardSquare square2;
        
        square2 = square1;
        
        assert(square2.getSquareColor() == RED);
        assert(square2.redRobotPresent() == true);
        assert(square2.blueRobotPresent() == false);
        assert(square2.robotDirection() == WEST);
        assert(square2.getSquareType() == ROCK);
        
        // Test self-assignment
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wself-assign-overloaded"
        square1 = square1;
        #pragma clang diagnostic pop
        
        assert(square1.getSquareColor() == RED);
        std::cout << "Assignment operator test: PASS\n";
    }

    /**
     * @test Tests whether the getter gets the correct square color 
     */
    void BoardSquareTest::testGetSquareColor() {
        BoardSquare square1(RED, false, false, NORTH, EMPTY);
        BoardSquare square2(BLUE, false, false, NORTH, EMPTY);
        BoardSquare square3(WHITE, false, false, NORTH, EMPTY);
        
        assert(square1.getSquareColor() == RED);
        assert(square2.getSquareColor() == BLUE);
        assert(square3.getSquareColor() == WHITE);
        std::cout << "GetSquareColor test: PASS\n";
    }

    /**
     * @test Tests whether getter returns the correct square type 
     */
    void BoardSquareTest::testGetSquareType() {
        BoardSquare square1(WHITE, false, false, NORTH, EMPTY);
        BoardSquare square2(WHITE, false, false, NORTH, ROCK);
        BoardSquare square3(WHITE, false, false, NORTH, FOG);
        BoardSquare square4(WHITE, false, false, NORTH, WALL);
        
        assert(square1.getSquareType() == EMPTY);
        assert(square2.getSquareType() == ROCK);
        assert(square3.getSquareType() == FOG);
        assert(square4.getSquareType() == WALL);
        std::cout << "GetSquareType test: PASS\n";
    }   

    /**
     * @test Tests whether getter correctly returns if a red robot is present 
     */
    void BoardSquareTest::testRedRobotPresent() {
        BoardSquare square1(WHITE, true, false, NORTH, EMPTY);
        BoardSquare square2(WHITE, false, false, NORTH, EMPTY);
        
        assert(square1.redRobotPresent() == true);
        assert(square2.redRobotPresent() == false);
        std::cout << "RedRobotPresent test: PASS\n";
    }

    /**
     * @test Tests whether getter correctly returns if a blue robot is present 
     */
    void BoardSquareTest::testBlueRobotPresent() {
        BoardSquare square1(WHITE, false, true, NORTH, EMPTY);
        BoardSquare square2(WHITE, false, false, NORTH, EMPTY);
        
        assert(square1.blueRobotPresent() == true);
        assert(square2.blueRobotPresent() == false);
        std::cout << "BlueRobotPresent test: PASS\n";
    }

    /**
     * @test Tests whether the getter correctly returns the correct robot direction
     */
    void BoardSquareTest::testRobotDirection() {
        BoardSquare square1(WHITE, false, false, NORTH, EMPTY);
        BoardSquare square2(WHITE, false, false, SOUTH, EMPTY);
        BoardSquare square3(WHITE, false, false, EAST, EMPTY);
        BoardSquare square4(WHITE, false, false, WEST, EMPTY);
        
        assert(square1.robotDirection() == NORTH);
        assert(square2.robotDirection() == SOUTH);
        assert(square3.robotDirection() == EAST);
        assert(square4.robotDirection() == WEST);
        std::cout << "RobotDirection test: PASS\n";
    }

    bool BoardSquareTest::doTests() {
        bool allTestsPassed = true;
        try {
            std::cout << "-------------------------\n"; 
            std::cout << "BoardSquare class unit tests\n";
            std::cout << "-------------------------\n";
            
            testDefaultConstructor();
            testSpecificConstructor();
            testCopyConstructor();
            testAssignmentOperator();
            testGetSquareColor();
            testGetSquareType();
            testRedRobotPresent();
            testBlueRobotPresent();
            testRobotDirection();
            
        } catch (const std::exception& e) {
            std::cout << "Test failed with exception: " << e.what() << std::endl;
            allTestsPassed = false;
        }
        
        return allTestsPassed;
    }   
}

int main(){
    Paintbots::BoardSquareTest tester; 
    return tester.doTests() ? 0 : 1; 
}