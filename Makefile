CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -pedantic -g

# Source files
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

# Test executable object files
CONFIGTEST_OBJS = ConfigTest.o Config.o InternalBoardSquare.o ExternalBoardSquare.o
BOARDSQUARETEST_OBJS = BoardSquareTest.o InternalBoardSquare.o ExternalBoardSquare.o
INTERNALBOARDSQUARETEST_OBJS = InternalBoardSquareTest.o InternalBoardSquare.o
EXTERNALBOARDSQUARETEST_OBJS = ExternalBoardSquareTest.o ExternalBoardSquare.o InternalBoardSquare.o
GAMEBOARDTEST_OBJS = GameBoardTest.o GameBoard.o InternalBoardSquare.o ExternalBoardSquare.o Config.o ObserverPattern.o
PLAINDISPLAYTEST_OBJS = PlainDisplayTest.o PlainDisplay.o GameBoard.o InternalBoardSquare.o ExternalBoardSquare.o Config.o ObserverPattern.o

# Main game object files
PAINTBOTS_OBJS = GameLogic.o GameBoard.o Config.o Robot.o RobotAgentRoster.o \
                  PlainDisplay.o InternalBoardSquare.o ExternalBoardSquare.o \
                  ObserverPattern.o LazyRobot.o RandomRobot.o AntiRandom.o

# All executables
EXECUTABLES = ConfigTest BoardSquareTest InternalBoardSquareTest \
               ExternalBoardSquareTest GameBoardTest PlainDisplayTest paintbots

# Default target
all: $(EXECUTABLES)

# Rules for compiling individual object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Rules for test executables
ConfigTest: $(CONFIGTEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

BoardSquareTest: $(BOARDSQUARETEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

InternalBoardSquareTest: $(INTERNALBOARDSQUARETEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

ExternalBoardSquareTest: $(EXTERNALBOARDSQUARETEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

GameBoardTest: $(GAMEBOARDTEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

PlainDisplayTest: $(PLAINDISPLAYTEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Rule for the main game executable
paintbots: $(PAINTBOTS_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Test target to build and run tests
test: ConfigTest BoardSquareTest InternalBoardSquareTest \
      ExternalBoardSquareTest GameBoardTest PlainDisplayTest
	@echo "Running tests..."
	@./ConfigTest && \
	 ./BoardSquareTest && \
	 ./InternalBoardSquareTest && \
	 ./ExternalBoardSquareTest && \
	 ./GameBoardTest && \
	 ./PlainDisplayTest

# Clean target
clean:
	rm -f *.o $(EXECUTABLES)

.PHONY: all clean test
