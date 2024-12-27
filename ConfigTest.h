#ifndef CONFIG_TEST_H
#define CONFIG_TEST_H

#include "ITest.h"
#include <string>
#include <cstring>  
#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {
    class ConfigTest : public ITest {
    private:
        // Helper methods
        void createTestFile(const std::string& filename, const std::string& content);
        void makeFileReadOnly(const std::string& filename);

        // Test methods
        void testDefaultConstructor();
        void testNonExistentFile();
        void testValidConfigFile();
        void testPartialConfigFile();
        void testInvalidFormat();
        void testInvalidValue();
        void testCaseInsensitivity();
        void testCommentAndEmptyLines();
        void testInvalidBounds();
        void testWhitespace();
        void testNegativeValues();
        void testZeroValues();
        void testLargeValues();
        void testFilePermissions();
        void testSpecialCharacters();
        void testMultipleEquals();
        void testDuplicateKeys();
        void testEmptyValue();
        void testRobotCollision();
        void testInvalidColorAssignment();
        void testRockFogConflict();
        void testBoardManipulation();
        void testConfigSingleton();

    public:
        bool doTests() override;
    };
}

#endif