#include "ConfigTest.h"
#include "Config.h"
#include "InternalBoardSquare.h"
#include "ExternalBoardSquare.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include <limits>
#include <sys/stat.h>
#include <cstring>  
#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {

/**
 * Helper method which creates files for testing 
 */
void ConfigTest::createTestFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    file << content;
}

/**
 * Helper method to make read-only files 
 */
void ConfigTest::makeFileReadOnly(const std::string& filename) {
    chmod(filename.c_str(), S_IRUSR);
}

/**
 * @test Tests the default constructor for Config 
 */
void ConfigTest::testDefaultConstructor() {
    Config& config = Config::getInstance();
    assert(config.getHitDuration() == 20);
    assert(config.getPaintBlobLimit() == 30);
    assert(config.getRockLowerBound() == 10);
    assert(config.getRockUpperBound() == 20);
    assert(config.getFogLowerBound() == 5);
    assert(config.getFogUpperBound() == 10);
    assert(config.getLongRangeLimit() == 30);
    std::cout << "Default constructor test: PASS\n";
}

/**
 * @test Tests program behavior when a non-existent config file is passed as
 * a parameter 
 */
void ConfigTest::testNonExistentFile() {
    try {
        Config::getInstance("nonexistent.txt");
        assert(false);
    } catch (const std::runtime_error&) {
        std::cout << "Non-existent file test: PASS\n";
    }
}

/**
 * @test Tests whether correct values are being set from a correct config file 
 */
void ConfigTest::testValidConfigFile() {
    const char* content = 
        "HIT_DURATION = 28\n"
        "PAINTBLOB_LIMIT = 25\n"
        "ROCK_LOWER_BOUND = 12\n"
        "ROCK_UPPER_BOUND = 15\n"
        "FOG_LOWER_BOUND = 3\n"
        "FOG_UPPER_BOUND = 8\n"
        "LONG_RANGE_LIMIT = 26\n";
    
    createTestFile("valid.txt", content);
    Config& config = Config::getInstance("valid.txt");
    assert(config.getHitDuration() == 28);
    assert(config.getPaintBlobLimit() == 25);
    assert(config.getRockLowerBound() == 12);
    assert(config.getRockUpperBound() == 15);
    assert(config.getFogLowerBound() == 3);
    assert(config.getFogUpperBound() == 8);
    assert(config.getLongRangeLimit() == 26);
    std::cout << "Valid config file test: PASS\n";
}

/**
 * @test Testing program behavior to handle partial config files 
 */
void ConfigTest::testPartialConfigFile() {
    const char* content = 
        "HIT_DURATION = 28\n"
        "PAINTBLOB_LIMIT = 25\n";
    
    createTestFile("partial.txt", content);
    Config& config = Config::getInstance("partial.txt");
    assert(config.getHitDuration() == 28);
    assert(config.getPaintBlobLimit() == 25);
    assert(config.getRockLowerBound() == 10);
    assert(config.getRockUpperBound() == 20);
    std::cout << "Partial config file test: PASS\n";
}

/**
 * @test Tests program behavior when the file passed in has an invalid format 
 */
void ConfigTest::testInvalidFormat() {
    const char* content = "HIT_DURATION 28\n";
    createTestFile("invalid_format.txt", content);
    
    try {
        Config::getInstance("invalid_format.txt");
        assert(false);
    } catch (const std::runtime_error&) {
        std::cout << "Invalid format test: PASS\n";
    }
}

/**
 * @test Tests program behavior when a file passed in has wrong config values 
 * (such as letters instead of numbers )
 */
void ConfigTest::testInvalidValue() {
    const char* content = "HIT_DURATION = abc\n";
    createTestFile("invalid_value.txt", content);
    
    try {
        Config::getInstance("invalid_value.txt");
        assert(false);
    } catch (const std::runtime_error&) {
        std::cout << "Invalid value test: PASS\n";
    }
}

/**
 * @test Tests program behavior when all of the words are in small letters 
 */
void ConfigTest::testCaseInsensitivity() {
    const char* content = "hit_duration = 28\nPaintBlob_Limit = 25\n";
    createTestFile("case.txt", content);
    
    Config& config = Config::getInstance("case.txt");
    assert(config.getHitDuration() == 28);
    assert(config.getPaintBlobLimit() == 25);
    std::cout << "Case insensitivity test: PASS\n";
}

/**
 * @test tests program behavior by varying comments and empty lines 
 */
void ConfigTest::testCommentAndEmptyLines() {
    const char* content = 
        "# This is a comment\n"
        "\n"
        "   \n"
        "HIT_DURATION = 28\n"
        "# Another comment\n"
        "PAINTBLOB_LIMIT = 25\n";
    
    createTestFile("comments.txt", content);
    Config& config = Config::getInstance("comments.txt");
    assert(config.getHitDuration() == 28);
    assert(config.getPaintBlobLimit() == 25);
    std::cout << "Comments and empty lines test: PASS\n";
}

/**
 * @test Tests program behavior when upper bound is lesser than lower bound 
 */
void ConfigTest::testInvalidBounds() {
    const char* content = 
        "ROCK_LOWER_BOUND = 20\n"
        "ROCK_UPPER_BOUND = 10\n";
    
    createTestFile("invalid_bounds.txt", content);
    try {
        Config::getInstance("invalid_bounds.txt");
        assert(false);
    } catch (const std::runtime_error&) {
        std::cout << "Invalid bounds test: PASS\n";
    }
}

/**
 * @test Tests program behavior with varying whitespace in config file 
 */
void ConfigTest::testWhitespace() {
    const char* content = "  HIT_DURATION   =   28  \n";
    createTestFile("whitespace.txt", content);
    Config& config = Config::getInstance("whitespace.txt");
    assert(config.getHitDuration() == 28);
    std::cout << "Whitespace handling test: PASS\n";
}

/**
 * @test Tests program behavior when negative values are suppled in config files 
 */
void ConfigTest::testNegativeValues() {
    const char* content = 
        "HIT_DURATION = -28\n"
        "PAINTBLOB_LIMIT = -25\n";
    createTestFile("negative.txt", content);
    try {
        Config::getInstance("negative.txt");
        assert(false);
    } catch (const std::runtime_error&) {
        std::cout << "Negative values test: PASS\n";
    }
}

/**
 * @test Tests when zeros are supplied as config values 
 */
void ConfigTest::testZeroValues() {
    const char* content = 
        "HIT_DURATION = 0\n"
        "PAINTBLOB_LIMIT = 0\n"
        "ROCK_LOWER_BOUND = 0\n"
        "ROCK_UPPER_BOUND = 0\n";
    createTestFile("zero.txt", content);
    try {
        Config::getInstance("zero.txt");
        assert(false);
    } catch (const std::runtime_error&) {
        std::cout << "Zero values test: PASS\n";
    }
}

/**
 * @test Tests program behavior when huge values are supplies as config values 
 */
void ConfigTest::testLargeValues() {
    const char* content = 
        "HIT_DURATION = 2147483648\n";
    createTestFile("large.txt", content);
    try {
        Config::getInstance("large.txt");
        assert(false);
    } catch (const std::runtime_error&) {
        std::cout << "Large values test: PASS\n";
    }
}

/**
 * @test Tests program behavior with varying access levels 
 */
void ConfigTest::testFilePermissions() {
    const char* content = "HIT_DURATION = 28\n";
    createTestFile("readonly.txt", content);
    makeFileReadOnly("readonly.txt");
    try {
        Config::getInstance("readonly.txt");
        std::cout << "Read-only file test: PASS\n";
    } catch (const std::runtime_error&) {
        std::cout << "Read-only file test: FAIL\n";
    }
}

/**
 * @test Tests program behavior when special characters are passed as config values 
 */
void ConfigTest::testSpecialCharacters() {
    const char* content = 
        "HIT_DURATION = 28\n"
        "PAINTBLOB@LIMIT = 25\n";
    createTestFile("special.txt", content);
    try {
        Config::getInstance("special.txt");
        assert(false);
    } catch (const std::runtime_error&) {
        std::cout << "Special characters test: PASS\n";
    }
}

/**
 * @test Tests program behavior when multiple equal signs are supplied in the config file 
 */
void ConfigTest::testMultipleEquals() {
    const char* content = "HIT_DURATION = 28 = 30\n";
    createTestFile("multiple_equals.txt", content);
    try {
        Config::getInstance("multiple_equals.txt");
        assert(false);
    } catch (const std::runtime_error&) {
        std::cout << "Multiple equals test: PASS\n";
    }
}

/**
 * @test Tests program behavior when duplicate keys are supplied as config values 
 * Program only takes in the latest value 
 */
void ConfigTest::testDuplicateKeys() {
    const char* content = 
        "HIT_DURATION = 28\n"
        "HIT_DURATION = 30\n";
    createTestFile("duplicate.txt", content);
    Config& config = Config::getInstance("duplicate.txt");
    assert(config.getHitDuration() == 30);
    std::cout << "Duplicate keys test: PASS\n";
}

/**
 * @test Tests program behavior when empty values are given as config values 
 */
void ConfigTest::testEmptyValue() {
    const char* content = "HIT_DURATION =\n";
    createTestFile("empty_value.txt", content);
    try {
        Config::getInstance("empty_value.txt");
        assert(false);
    } catch (const std::runtime_error&) {
        std::cout << "Empty value test: PASS\n";
    }
}

/**
 * @test Tests program behavior when robots collide 
 */
void ConfigTest::testRobotCollision() {
    InternalBoardSquare square;
    square.setRedRobot(true);
    try {
        square.setBlueRobot(true);
        assert(false);
    } catch (const std::runtime_error&) {
        std::cout << "Robot collision test: PASS\n";
    }
}

/**
 * @test Tests program behavior when invalid colors are assigned to robots 
 */
void ConfigTest::testInvalidColorAssignment() {
    InternalBoardSquare square;
    try {
        square.setSquareColor(static_cast<SquareColor>(999));
        assert(false);
    } catch (const std::runtime_error&) {
        std::cout << "Invalid color test: PASS\n";
    }
}

/**
 * TODO: Rock and fog can be placed on the same square, need to change this 
 * @test Tests program behavior when rock and fog are placed on the same square 
 */
void ConfigTest::testRockFogConflict() {
    InternalBoardSquare square;
    square.setType(ROCK);
    try {
        square.setType(FOG);
        assert(false);
    } catch (const std::runtime_error&) {
        std::cout << "Rock-Fog conflict test: PASS\n";
    }
}

/**
 * @test Tests program behavior when test board is manipulated 
 */
void ConfigTest::testBoardManipulation() {
    InternalBoardSquare internal;
    internal.setSquareColor(RED);
    internal.setRedRobot(true);
    ExternalBoardSquare external(internal);
    assert(external.getSquareColor() == RED);
    assert(external.redRobotPresent());
    std::cout << "Board manipulation test: PASS\n";
}

/**
 * @test Testing singleton behvior of config class 
 */
void ConfigTest::testConfigSingleton() {
    Config& config1 = Config::getInstance();
    Config& config2 = Config::getInstance();
    assert(&config1 == &config2);
    std::cout << "Config singleton test: PASS\n";
}

bool ConfigTest::doTests() {
    bool allTestsPassed = true;
    try {
        std::cout << "----------------------\n";
        std::cout << "Config class unit tests\n";
        std::cout << "----------------------\n";
        
        testDefaultConstructor();
        testNonExistentFile();
        testValidConfigFile();
        testPartialConfigFile();
        testInvalidFormat();
        testInvalidValue();
        testCaseInsensitivity();
        testCommentAndEmptyLines();
        testInvalidBounds();
        testWhitespace();
        testNegativeValues();
        testZeroValues();
        testLargeValues();
        testFilePermissions();
        testSpecialCharacters();
        testMultipleEquals();
        testDuplicateKeys();
        testEmptyValue();
        testRobotCollision();
        testInvalidColorAssignment();
        testRockFogConflict();
        testBoardManipulation();
        testConfigSingleton();
        
    } catch (const std::exception& e) {
        std::cout << "Test failed with exception: " << e.what() << std::endl;
        allTestsPassed = false;
    }
    
    return allTestsPassed;
}

} 

int main() {
    Paintbots::ConfigTest tester;
    return tester.doTests() ? 0 : 1;
}