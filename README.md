# Simplified Checkers (C)

## Overview
A terminal-based implementation of a simplified Checkers game written in C (C99).
The program supports two-player turn-based gameplay with strict rule enforcement,
capture mechanics, automated multi-captures, multiple win conditions, and score tracking.

This project emphasizes modular program design, state management without global variables,
and robust input validation. It is the

## Features
- 8×8 checkerboard using ASCII representation (black squares only)
- Randomized first player selection using `rand()` (coin-flip logic)
- Two-player turn-based gameplay
- Diagonal forward movement enforcement
- Single capture per move (base requirement)
- Automated multi-capture chaining when additional captures are possible
- Real-time piece count updates for both players
- Surrender option during gameplay
- Score tracking across multiple games within a session

## Win Conditions
A player wins when any of the following occur:
- One of their pieces reaches the opponent’s end row
- All opponent pieces are captured
- The opponent has no remaining legal moves
- The opponent surrenders

## Bonus Feature (Beyond Requirements)
In addition to the base project specifications, the game supports **automatic multiple captures**.
After a successful capture, the program checks whether further captures are possible and
executes them automatically until no additional captures are available.

This feature required additional board state analysis and move validation logic beyond
the minimum requirements.

- Language: C (C99)
- Compiler: gcc
- Platform: Terminal / Command Line

## Project Context
This project was developed as part of **CCPROG1 - Logic Formulation and Introductory Programming**, an introductory programming course at De La Salle University.
The implementation followed strict constraints, including:
- No global or static variables
- No `goto`, `exit`, or premature `return` statements
- No calling `main()` recursively
- Modular, function-based design with parameter passing
- Manual memory and state management using arrays

The original project specification is available in `specs/MP - Simple Checkers.pdf`.

## Design Highlights
- Board state is represented using a 2D array to simplify coordinate tracking
- Piece movement and captures are validated through dedicated functions
- Game flow is controlled using explicit state variables instead of early exits
- Multi-capture logic is isolated into a separate function for clarity and reuse
- Defensive input handling prevents invalid or malformed user input from breaking the game loop
