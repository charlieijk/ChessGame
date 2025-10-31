#!/bin/bash
# Simple build script for Chess Game

echo "Building Chess Game..."

# Detect SFML installation path
if [ -d "/opt/homebrew/opt/sfml" ]; then
    # Apple Silicon Homebrew path
    SFML_PREFIX="/opt/homebrew/opt/sfml"
elif [ -d "/usr/local/opt/sfml" ]; then
    # Intel Homebrew path
    SFML_PREFIX="/usr/local/opt/sfml"
else
    echo "Warning: SFML not found in standard Homebrew locations. Attempting default compilation..."
    SFML_PREFIX=""
fi

# Compile the chess game
if [ -n "$SFML_PREFIX" ]; then
    clang++ -std=c++17 -Wall chess.cpp -o chess \
        -I"$SFML_PREFIX/include" \
        -L"$SFML_PREFIX/lib" \
        -lsfml-graphics -lsfml-window -lsfml-system \
        -Wl,-rpath,"$SFML_PREFIX/lib"
else
    clang++ -std=c++17 -Wall chess.cpp -o chess -lsfml-graphics -lsfml-window -lsfml-system
fi

if [ $? -eq 0 ]; then
    echo "Build successful! Run './chess' to start the game."
else
    echo "Build failed. Make sure SFML is installed."
    echo "On macOS: brew install sfml"
    echo "On Ubuntu/Debian: sudo apt-get install libsfml-dev"
    exit 1
fi
