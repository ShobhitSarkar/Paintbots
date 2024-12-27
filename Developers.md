# Paintbots Project Developer Guide

## Overview
Paintbots is a strategic grid-based game where two robots compete to paint the most squares on a 15x15 game board. The project is designed with a focus on modular architecture, robust error handling, and extensible game mechanics.

## Core Architecture

### Design Patterns
1. **Singleton Pattern**
   - Used in `Config`, `GameBoard`, and `RobotAgentRoster` classes
   - Ensures single, consistent instance of critical game components
   - Provides global access point with controlled instantiation

2. **Strategy Pattern**
   - Implemented in `Robot` and `IRobotAgent`
   - Allows dynamic swapping of robot behaviors
   - Supports easy addition of new robot strategies

3. **Observer Pattern**
   - Implemented through `Observable` and `Observer` base classes
   - Used for board state notifications
   - Allows decoupled updates to display and other game components

4. **Inheritance and Polymorphism**
   - `BoardSquare` base class with `InternalBoardSquare` and `ExternalBoardSquare` derived classes
   - Provides controlled access to game state

## Key Components

### BoardSquare Hierarchy
```cpp
class BoardSquare {
protected:
    SquareColor squareColor;
    bool hasRedRobot, hasBlueRobot;
    Direction robotFacing;
    SquareType squareType;
};

class InternalBoardSquare : public BoardSquare {
public:
    void setSquareColor(SquareColor color);
    void setRedRobot(bool present);
    void setBlueRobot(bool present);
    void setRobotDirection(Direction dir);
    void setType(SquareType sqType);
};

class ExternalBoardSquare : public BoardSquare {
    // Read-only interface for robot agents
};
```

### Configuration Management
```cpp
class Config {
private:
    static Config* instance;
    // Configuration parameters
    int hitDuration;
    int paintBlobLimit;
    int rockLowerBound, rockUpperBound;
    int fogLowerBound, fogUpperBound;
    int longRangeLimit;

public:
    static Config& getInstance(const std::string& configFile = "");
    int getHitDuration() const;
    // Getter methods for other parameters
};
```

### Game Board Management
```cpp
class GameBoard : public Observable {
private:
    static GameBoard* instance;
    static const std::string PASSWORD;
    
    InternalBoardSquare board[BOARD_SIZE + 2][BOARD_SIZE + 2];
    
    // Game state tracking
    int redPaintBlobsRemaining;
    int bluePaintBlobsRemaining;
    int redRobotHitDuration;
    int blueRobotHitDuration;

public:
    static GameBoard* getInstance(const std::string& password);
    bool moveRobot(RobotMoveRequest& mr);
    bool paintBlobHit(RobotMoveRequest& mr);
    ExternalBoardSquare** getShortRangeScan(RobotColor rc);
};
```

### Robot Agents
```cpp
class IRobotAgent {
public:
    virtual RobotMoveRequest* getMove(
        ExternalBoardSquare** shortRangeScan, 
        ExternalBoardSquare** longRangeScan
    ) = 0;
    virtual void setRobotColor(RobotColor c) = 0;
};

class RobotAgentRoster {
private:
    static RobotAgentRoster* instance;
    std::vector<IRobotAgent*> agents;

public:
    void add(IRobotAgent* agent);
    IRobotAgent* operator[](int index);
    IRobotAgent* operator[](const std::string& name);
};
```

## Error Handling Strategies

### Configuration Validation
- Custom exception hierarchy (`ConfigError`, `ConfigFileError`, etc.)
- Validates:
  - File existence
  - Value ranges
  - Configuration format
  - Bounds consistency

### Game State Validation
- Prevents:
  - Robot collisions
  - Invalid square type combinations
  - Out-of-bounds movements
  - Illegal paint blob shots

## Memory Management
- Smart pointer usage in key components
- Careful memory cleanup in destructors
- Singleton pattern prevents memory leaks
- Custom deleters in complex objects

## Testing Approach
- Comprehensive test suites for each component
- Unit tests covering:
  - Initialization
  - State changes
  - Error scenarios
  - Edge cases
- Uses `ITest` interface for consistent testing

## Performance Considerations
- Random number generation with `<random>` library
- Efficient board scanning techniques
- Minimal memory allocation during game loop
- Constant-time singleton access

## Extensibility Points
1. Easy addition of new robot strategies
2. Configurable board parameters
3. Modular display system
4. Flexible observer notifications

## Recommended Improvements
1. Dynamic board size configuration
2. Enhanced AI for robot strategies
3. More comprehensive logging
4. Network multiplayer support

## Coding Standards
- C++11 compliant
- RAII principles
- Const-correctness
- Minimal global state
- Comprehensive error handling

## Build and Development
- Makefile for cross-platform compilation
- Modular project structure
- Separate test executables
- Dependency tracking

## Running 
Refer to the ReadMe file for building, compiling and running instructions 

## Author Information
- Name: Shobhit Sarkar
- Course: COM S 327, Iowa State University
- Semester: Fall 2024