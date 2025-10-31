#!/bin/bash
# Simple build script for Chess Game

echo "Building Chess Game..."

# Check if SFML is installed
if ! command -v pkg-config &> /dev/null || ! pkg-config --exists sfml-all; then
    echo "Warning: SFML may not be installed. Attempting to compile anyway..."
fi

# Compile the chess game
clang++ -std=c++17 -Wall chess.cpp -o chess -lsfml-graphics -lsfml-window -lsfml-system

if [ $? -eq 0 ]; then
    echo "Build successful! Run './chess' to start the game."
else
    echo "Build failed. Make sure SFML is installed."
    echo "On macOS: brew install sfml"
    echo "On Ubuntu/Debian: sudo apt-get install libsfml-dev"
    exit 1
fi
