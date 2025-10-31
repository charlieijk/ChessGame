# Chess Game

A graphical chess game built with C++ and SFML.

## Prerequisites

You need to have SFML (Simple and Fast Multimedia Library) installed on your system.

### Installing SFML

**macOS:**
```bash
brew install sfml
```

**Ubuntu/Debian:**
```bash
sudo apt-get install libsfml-dev
```

**Windows:**
Download SFML from [https://www.sfml-dev.org/download.php](https://www.sfml-dev.org/download.php)

## Compilation

There are three easy ways to compile this project:

### Option 1: Using the build script (Easiest)
```bash
./build.sh
```

### Option 2: Using Make
```bash
make
```

To clean build artifacts:
```bash
make clean
```

To build and run:
```bash
make run
```

### Option 3: Manual compilation
```bash
clang++ -std=c++17 -Wall chess.cpp -o chess -lsfml-graphics -lsfml-window -lsfml-system
```

## Running the Game

After compilation, run:
```bash
./chess
```

## Troubleshooting

If you get compilation errors about SFML not being found:
1. Make sure SFML is properly installed
2. On some systems, you may need to specify the SFML path manually
3. Check that pkg-config can find SFML: `pkg-config --libs sfml-all`
