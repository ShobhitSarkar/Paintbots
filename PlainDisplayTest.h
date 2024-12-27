#ifndef PLAIN_DISPLAY_TEST_H
#define PLAIN_DISPLAY_TEST_H

#include "ITest.h"
#include "GameBoard.h"
#include <sstream>
#include <string>
#include <cstring>  

#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {

class PlainDisplayTest : public ITest {
private:
    // Helper class for capturing cout output
    class OutputCapture {
        std::ostringstream capture;  
        std::streambuf* oldCout;    
    public:
        OutputCapture();
        ~OutputCapture();
        std::string getOutput();
    };

    // Helper methods
    GameBoard* getTestBoard();
    void setUp();
    void tearDown();

    // Test methods
    void testInitialization();
    void testWallDisplay();
    void testRobotDisplay();
    void testRockDisplay();
    void testFogDisplay();
    void testColorDisplay();
    void testObserverUpdate();
    void testCoordinateDisplay();
    void testMultipleUpdates();
    void testComplexBoardState();
    void testMemoryManagement();

public:
    // Required by ITest interface
    bool doTests() override;
};

} // namespace Paintbots

#endif // PLAIN_DISPLAY_TEST_H