#include <iostream>
#include <vector>
#include <memory>
#include <string>

// Enums for piece types and colors
enum class PieceType {
    PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING, NONE
};

enum class Color {
    WHITE, BLACK, NONE
};

// Position structure to represent board coordinates
struct Position {
    int row;
    int col;
    
    Position(int r = 0, int c = 0) : row(r), col(c) {}
    bool isValid() const;
};

// Base Piece class
class Piece {
protected:
    Color color;
    PieceType type;
    Position position;
    bool hasMoved;
    
public:
    Piece(Color c, PieceType t, Position pos);
    virtual ~Piece() = default;
    
    // Getters
    Color getColor() const;
    PieceType getType() const;
    Position getPosition() const;
    bool hasMovedBefore() const;
    
    // Setters
    void setPosition(Position pos);
    void setHasMoved(bool moved);
    
    // Pure virtual function - each piece implements its own move logic
    virtual bool isValidMove(Position from, Position to, class Board& board) const = 0;
    
    // Virtual function for getting piece symbol
    virtual char getSymbol() const = 0;
};

// Derived piece classes
class Pawn : public Piece {
public:
    Pawn(Color c, Position pos);
    bool isValidMove(Position from, Position to, Board& board) const override;
    char getSymbol() const override;
};

class Rook : public Piece {
public:
    Rook(Color c, Position pos);
    bool isValidMove(Position from, Position to, Board& board) const override;
    char getSymbol() const override;
};

class Knight : public Piece {
public:
    Knight(Color c, Position pos);
    bool isValidMove(Position from, Position to, Board& board) const override;
    char getSymbol() const override;
};

class Bishop : public Piece {
public:
    Bishop(Color c, Position pos);
    bool isValidMove(Position from, Position to, Board& board) const override;
    char getSymbol() const override;
};

class Queen : public Piece {
public:
    Queen(Color c, Position pos);
    bool isValidMove(Position from, Position to, Board& board) const override;
    char getSymbol() const override;
};

class King : public Piece {
public:
    King(Color c, Position pos);
    bool isValidMove(Position from, Position to, Board& board) const override;
    char getSymbol() const override;
};

// Board class
class Board {
private:
    std::vector<std::vector<std::shared_ptr<Piece>>> board;
    
public:
    Board();
    
    // Initialize the board with pieces in starting positions
    void initialize();
    
    // Get piece at position
    std::shared_ptr<Piece> getPiece(Position pos) const;
    
    // Set piece at position
    void setPiece(Position pos, std::shared_ptr<Piece> piece);
    
    // Check if position is empty
    bool isEmpty(Position pos) const;
    
    // Check if path is clear (for rook, bishop, queen moves)
    bool isPathClear(Position from, Position to) const;
    
    // Display the board
    void display() const;
};

// Move structure
struct Move {
    Position from;
    Position to;
    std::shared_ptr<Piece> capturedPiece;
    
    Move(Position f, Position t) : from(f), to(t), capturedPiece(nullptr) {}
};

// Game class - manages the game state
class Game {
private:
    Board board;
    Color currentPlayer;
    std::vector<Move> moveHistory;
    bool gameOver;
    
public:
    Game();
    
    // Start the game
    void start();
    
    // Make a move
    bool makeMove(Position from, Position to);
    
    // Validate move
    bool isValidMove(Position from, Position to) const;
    
    // Check if king is in check
    bool isInCheck(Color color) const;
    
    // Check if king is in checkmate
    bool isCheckmate(Color color) const;
    
    // Check if game is stalemate
    bool isStalemate(Color color) const;
    
    // Switch player turn
    void switchPlayer();
    
    // Get current player
    Color getCurrentPlayer() const;
    
    // Game loop
    void play();
    
    // Undo last move
    void undoMove();
};

// Main function
int main() {
    Game game;
    game.start();
    game.play();
    
    return 0;
};