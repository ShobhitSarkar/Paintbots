# coms3270P2

## Overview

 Players create and deploy robot agents with unique strategies to outmaneuver their opponent, painting squares and blocking their path.

## Game Mechanics

### Core Gameplay
- 15x15 grid battlefield
- Two robots (red and blue) compete simultaneously
- Objective: Paint more squares than the opponent
- 300 total moves per game
- Multiple game elements add strategic depth:
  - Rocks blocking movement
  - Fog squares limiting visibility
  - Paint blob shooting mechanism
  - Limited paint blob shots

### Unique Features
- Dynamic board generation
- Configurable game parameters
- Multiple robot strategies
- Advanced movement and shooting mechanics
- Realistic collision and interaction rules

## Key Components

### Board Management
- Intelligent square tracking
- Separate internal and external board representations
- Support for different square types:
  - Empty
  - Rock
  - Fog
  - Wall

### Robot Strategies
Built-in robot strategies:
1. **LazyRobot**: Minimalist movement, random shooting
2. **RandomRobot**: Unpredictable movement patterns
3. **BeatRandom**: Advanced strategy designed to outperform random movement

### Configuration
- Fully configurable game parameters
- Support for custom configuration files
- Default and custom rock/fog placements
- Configurable paint blob mechanics

## Prerequisites

### System Requirements
- C++ Compiler with C++11 support

## Installation and Setup

### Cloning the Repository
```bash
git clone https://github.com/your-username/paintbots.git
cd paintbots
```

### Compilation

#### Build All Components
```bash
make all
```

#### Build Specific Targets
```bash
# Build test executables
make test

# Build main game executable
make paintbots

# Build specific test suites
make ConfigTest
make BoardSquareTest
make GameBoardTest
```

#### Clean Build
```bash
make clean
```

## Running the Game

### Game Execution
```bash
# Basic syntax
./paintbots <board_config_file> <robot_config_file>

# Example
./paintbots board.config robots.config
```

### Configuration Files

#### Board Configuration (`board.config`)
```
# Sample board.config
HIT_DURATION = 25
PAINTBLOB_LIMIT = 20
ROCK_LOWER_BOUND = 5
ROCK_UPPER_BOUND = 15
FOG_LOWER_BOUND = 3
FOG_UPPER_BOUND = 8
LONG_RANGE_LIMIT = 5
```

#### Robot Configuration (`robots.config`)
```
# First line: Red robot strategy
# Second line: Blue robot strategy
BeatRandom
RandomRobot
```

## Running Tests

### Unit Tests
```bash
# Run all tests
make test

# Run specific test suites
./ConfigTest
./BoardSquareTest
./GameBoardTest
```

## Troubleshooting

### Common Issues
- Compilation Errors: Ensure C++11 compiler
- Runtime Errors: Check configuration files
- Test Failures: Verify system requirements

## Contact
- Project Maintainer: Shobhit Sarkar
- Email: [shobhit@iastate.edu]
- Course: COM S 327, Iowa State University
