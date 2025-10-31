# Chess Game Makefile
# Simple compilation for SFML-based chess game

CXX = clang++
CXXFLAGS = -std=c++17 -Wall
TARGET = chess
SOURCE = chess.cpp

# SFML library flags (automatically detects system libraries)
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Default target
all: $(TARGET)

# Compile the chess game
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET) $(LDFLAGS)

# Clean build artifacts
clean:
	rm -f $(TARGET)

# Run the game
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
