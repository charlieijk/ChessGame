# Chess Game Makefile
# Simple compilation for SFML-based chess game

CXX = clang++
CXXFLAGS = -std=c++17 -Wall
TARGET = chess
SOURCE = chess.cpp

# Detect SFML installation path
SFML_PREFIX := $(shell if [ -d "/opt/homebrew/opt/sfml" ]; then echo "/opt/homebrew/opt/sfml"; elif [ -d "/usr/local/opt/sfml" ]; then echo "/usr/local/opt/sfml"; fi)

# SFML library flags
ifneq ($(SFML_PREFIX),)
    CXXFLAGS += -I$(SFML_PREFIX)/include
    LDFLAGS = -L$(SFML_PREFIX)/lib -lsfml-graphics -lsfml-window -lsfml-system -Wl,-rpath,$(SFML_PREFIX)/lib
else
    LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
endif

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
