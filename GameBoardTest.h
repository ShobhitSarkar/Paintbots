#ifndef GAME_BOARD_TEST_H
#define GAME_BOARD_TEST_H

#include "ITest.h"
#include "GameBoard.h"
#include "ObserverPattern.h"
#include "BoardSquare.h"
#include "Config.h"
#include <string>
#include <cstring> 
#include <stdexcept>  
#include <algorithm>  

namespace Paintbots {
    class GameBoardTest : public ITest {
        private:

            /**
             * Helper method to create custom files 
             */
            void createTestConfig(const std::string& filename, const std::string& content);

            /**
             * Method to get the instantiated test board 
             */
            GameBoard* getTestBoard();

            /**
             * Verifies if the board dimensions are according to the specifications 
             */
            void verifyBoardDimensions(GameBoard* board);

            /**
             * Verifies whether a square is empty 
             */
            void verifyEmptySquare(int row, int col);

            /**
             * Verifies if a certain square is a valid position 
             */
            bool isValidPosition(int row, int col) const;

            /**
             * Instantiates a board for testing purposes 
             */
            void setUp() {
                GameBoard::resetInstance();
                GameBoard* board = getTestBoard();
                if (board == nullptr) {
                    throw std::runtime_error("Failed to get GameBoard instance");
                }

                // Reset observers
                for (Observer* observer : board->getObservers()) {
                    board->removeObserver(observer);
                }

                // Create a fresh instance with default config to reset paint blob counts
                Config::resetInstance();
                GameBoard::resetInstance();
                board = getTestBoard();

                // Reset board state
                for (int i = 1; i <= GameBoard::BOARD_SIZE; ++i) {
                    for (int j = 1; j <= GameBoard::BOARD_SIZE; ++j) {
                        // Only reset robots and colors, preserve walls/rocks/fog
                        if (board->getSquare(i, j).redRobotPresent()) {
                            board->getSquare(i, j).setRedRobot(false);
                        }
                        if (board->getSquare(i, j).blueRobotPresent()) {
                            board->getSquare(i, j).setBlueRobot(false);
                        }
                        if (board->getSquare(i, j).getSquareType() == EMPTY) {
                            board->getSquare(i, j).setSquareColor(WHITE);
                        }
                    }
                }

                // Place robots in initial positions
                bool placedRed = false;
                bool placedBlue = false;
                
                for (int i = 1; i <= GameBoard::BOARD_SIZE && (!placedRed || !placedBlue); ++i) {
                    for (int j = 1; j <= GameBoard::BOARD_SIZE && (!placedRed || !placedBlue); ++j) {
                        if (!placedRed && board->getSquare(i, j).getSquareType() == EMPTY) {
                            board->getSquare(i, j).setRedRobot(true);
                            board->getSquare(i, j).setRobotDirection(NORTH);
                            placedRed = true;
                            continue;
                        }
                        if (!placedBlue && board->getSquare(i, j).getSquareType() == EMPTY) {
                            board->getSquare(i, j).setBlueRobot(true);
                            board->getSquare(i, j).setRobotDirection(NORTH);
                            placedBlue = true;
                        }
                    }
                }
            }
            
            /**
             * Method to dismantle test set up 
             */
            void tearDown() {
                GameBoard* board = GameBoard::getInstance("xyzzy");
                if (board != nullptr) {
                    for (Observer* observer : board->getObservers()) {
                        board->removeObserver(observer);
                    }
                }
                GameBoard::resetInstance();
            }

            // 1. Singleton Pattern Tests
            void testGetInstanceWithPassword();
            void testGetInstanceWithWrongPassword();
            void testSingletonBehavior();
            void testPasswordProtection();

            // 2. Testing for correct board initialization 
            void testBoardDimensions();
            void testWallPlacement();
            void testRockPlacement();
            void testFogPlacement();
            void testRobotInitialPlacement();
            void testRobotPlacementValidation();

            // 3. tests for testing robot movement 
            void testBasicRobotMovements();
            void testWallCollisionPrevention();
            void testRockCollisionPrevention();
            void testFogMovement();
            void testBoardBoundaryPrevention();
            void testColorTrails();
            void testRobotCollisionPrevention();

            // 4. testing paintblob 
            void testPaintBlobHit();
            void testPaintBlobMiss();
            void testPaintBlobRockBlocking();
            void testPaintBlobFogBehavior();
            void testPaintBlobWallCollision();
            void testPaintBlobRange();
            void testPaintBlobDirection();

            // 5. Tests for score tracking 
            void testInitialScore();
            void testScoreIncrementOnMovement();
            void testScoreUpdateOnPaintBlob();
            void testScoreAfterMultipleOperations();
            
            // 6. Tests for short range scans 
            void testShortRangeScanBasic();
            void testShortRangeScanRobotPosition();
            void testShortRangeScanDirections();
            void testShortRangeScanWallBoundaries();
            void testShortRangeScanFogBehavior();
            void testShortRangeScanObstructions();
            
            // 7. Long range scan tests: 
            void testLongRangeDimensions();
            void testLongRangeWalls();
            void testLongRangeRocks();
            void testLongRangeFog();
            void testLongRangeRobots();
            void testLongRangeColors();

            // 8. Observer pattern tests
            void testMovementNotification();
            void testColorChangeNotification();
            void testScoreChangeNotification();
            void testObserverManagement();
            void testMultipleObservers();

            // 9. Error handling tests
            void testInvalidSquareAccess();
            void testInvalidRobotMovement();
            void testInvalidPaintBlobRequest();
            // void testInvalidColorAssignment();
            void testInvalidObserverOperations();
            void testInvalidScanRequests();
            void testMemoryLeakPrevention();
            void testConcurrentModificationHandling();
            void testNullPointerHandling();
            void testExceptionPropagation();

            /**
             * tests when other robot is hit 
             */
            void testPaintBlobHitColorChange();
            void testPaintBlobHitDuration();
            void testPaintBlobHitExpiration();
            void testMultipleRobotHits();

            void testPaintBlobLimit();
            // void testPaintBlobLimitReset();

            // Test fixture 
            class MockObserver : public Observer {
                public:
                    int updateCount = 0;
                    void update(Observable* subject) override { updateCount++; }
                    ~MockObserver() override = default; 
            };
        
        public:
            bool doTests() override;
    };
}

#endif