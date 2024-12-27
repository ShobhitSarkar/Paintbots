# Paintbots: A Strategic Robot Combat Game

## Overview

Paintbots is a sophisticated C++ game engine that implements a territory control game where two AI-controlled robots compete to paint squares on a grid-based board. The project demonstrates advanced software engineering principles, utilizing multiple design patterns and robust architecture to create a flexible and maintainable system.

## Core Architecture

### Design Patterns

The project implements several key design patterns to ensure maintainability, flexibility, and robust error handling:

1. **Singleton Pattern**
   - Implemented in:
     - `Config`: Manages game configuration
     - `GameBoard`: Controls game state
     - `RobotAgentRoster`: Manages robot strategies
   - Ensures single, consistent instances of critical components
   - Thread-safe implementation with lazy initialization

2. **Strategy Pattern**
   - Core implementation through `IRobotAgent` interface
   - Allows dynamic swapping of robot behaviors:
     - `LazyRobot`: Minimalist movement strategy
     - `RandomRobot`: Randomized decision making
     - `AntiRandom`: Counter-random strategy
   - Facilitates easy addition of new robot behaviors

3. **Observer Pattern**
   - Implemented via `Observable` and `Observer` base classes
   - Used for real-time board state notifications
   - Enables decoupled updates between components
   - Primary observers:
     - `PlainDisplay`: Console visualization
     - Custom observers can be added

4. **Factory Pattern**
   - Used in robot creation and configuration
   - Centralizes object creation logic
   - Supports runtime strategy selection

### Component Hierarchy

```
GameBoard
├── BoardSquare (Abstract)
│   ├── InternalBoardSquare
│   └── ExternalBoardSquare
├── Robot
│   └── IRobotAgent (Interface)
│       ├── LazyRobot
│       ├── RandomRobot
│       └── AntiRandom
└── Config
```

## Key Features

### Board Management
- 15x15 grid battlefield
- Multiple square types:
  - Empty squares
  - Rocks (movement obstacles)
  - Fog (visibility limitation)
  - Walls (board boundaries)
- Dynamic board generation
- Collision detection system

### Robot Control System
- Comprehensive robot API through `IRobotAgent`
- Multiple vision modes:
  - Short-range scan (5x5 grid)
  - Long-range scan (full board)
- Paint blob shooting mechanics
- Configurable AI strategies

### Configuration System
- External configuration file support
- Runtime parameter adjustment
- Configurable elements:
  - Hit duration
  - Paint blob limits
  - Rock/fog distribution
  - Scan ranges

## Building and Running

### Prerequisites
- C++11 compatible compiler
- Make build system
- Standard library support

### Build Commands
```bash
# Build all components
make all

# Build specific components
make paintbots    # Main game
make ConfigTest   # Configuration tests
make BoardTest    # Board component tests

# Clean build files
make clean
```

### Running the Game
```bash
./paintbots <board_config> <robot_config>
```

#### Configuration Files

board.config:
```
HIT_DURATION = 25
PAINTBLOB_LIMIT = 20
ROCK_LOWER_BOUND = 5
ROCK_UPPER_BOUND = 15
FOG_LOWER_BOUND = 3
FOG_UPPER_BOUND = 8
LONG_RANGE_LIMIT = 5
```

robots.config:
```
RandomRobot  # Red robot strategy
LazyRobot    # Blue robot strategy
```

## Testing Framework

### Unit Tests
- Comprehensive test suites for all components
- Test classes inherit from `ITest` interface
- Coverage includes:
  - Configuration validation
  - Board state management
  - Robot movement mechanics
  - Observer pattern functionality

### Test Execution
```bash
# Run all tests
make test

# Run specific test suite
./ConfigTest
./BoardSquareTest
./GameBoardTest
```

## Error Handling

### Robust Exception System
- Custom exception hierarchy
- Specific exception types:
  - `ConfigError`
  - `ConfigFileError`
  - `ConfigBoundsError`
  - `ConfigValueError`

### Validation Systems
- Configuration validation
- Move validation
- Board state validation
- Robot collision detection

## Memory Management

### RAII Principles
- Smart pointer usage
- Automatic resource cleanup
- Exception-safe design

### Resource Management
- Proper cleanup in destructors
- Memory leak prevention
- Efficient board state updates

## Contributing

### Adding New Robot Strategies
1. Inherit from `IRobotAgent`
2. Implement required methods:
   - `getRobotName()`
   - `getRobotCreator()`
   - `getMove()`
   - `setRobotColor()`
3. Register strategy in `RobotAgentRoster`

### Code Style
- Follow existing naming conventions
- Use consistent indentation
- Document public interfaces
- Include unit tests

## Authors and Acknowledgment
- Project Author: Shobhit Sarkar
