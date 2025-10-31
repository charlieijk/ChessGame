#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp> 



const int SQUARE_SIZE = 80; 

const int BOARD_SIZE = 8;
const int WINDOW_SIZE = SQUARE_SIZE * BOARD_SIZE;


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
    bool isValid() const{ 
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }
};

class ChessGUI { 
    private: 
    sf::RenderWindow window; 
    sf::Font font; 

    sf::Color lightSquare;
    sf::Color darkSquare;

    public:
        ChessGUI() : window(sf::VideoMode({WINDOW_SIZE, WINDOW_SIZE}), "Chess Game"),
        lightSquare(240, 217, 181),
        darkSquare(181, 136, 99) {

        }

       void run() {
    while (window.isOpen()) {
        handleEvents();
        render();
    }
}

private:
    void handleEvents() {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
    }
    
    void render() {
        window.clear();
        drawBoard();
        window.display();
    }
    
    void drawBoard() {
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                sf::RectangleShape square(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                square.setPosition(sf::Vector2f(col * SQUARE_SIZE, row * SQUARE_SIZE));
                
                if ((row + col) % 2 == 0) {
                    square.setFillColor(lightSquare);
                } else {
                    square.setFillColor(darkSquare);
                }
                
                window.draw(square);
            }
        }
    }
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

// After your class declarations, add these:

// Pawn
Pawn::Pawn(Color c, Position pos) : Piece(c, PieceType::PAWN, pos) {}
char Pawn::getSymbol() const { return (color == Color::WHITE) ? 'P' : 'p'; }

// Rook
Rook::Rook(Color c, Position pos) : Piece(c, PieceType::ROOK, pos) {}
char Rook::getSymbol() const { return (color == Color::WHITE) ? 'R' : 'r'; }

// Knight
Knight::Knight(Color c, Position pos) : Piece(c, PieceType::KNIGHT, pos) {}
char Knight::getSymbol() const { return (color == Color::WHITE) ? 'N' : 'n'; }

// Bishop
Bishop::Bishop(Color c, Position pos) : Piece(c, PieceType::BISHOP, pos) {}
char Bishop::getSymbol() const { return (color == Color::WHITE) ? 'B' : 'b'; }

// Queen
Queen::Queen(Color c, Position pos) : Piece(c, PieceType::QUEEN, pos) {}
char Queen::getSymbol() const { return (color == Color::WHITE) ? 'Q' : 'q'; }

// King
King::King(Color c, Position pos) : Piece(c, PieceType::KING, pos) {}
char King::getSymbol() const { return (color == Color::WHITE) ? 'K' : 'k'; }

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

void Board::initialize() { 
    for (int i = 0; i < 8; i++) { 
        for (int j = 0; j < 8; j++){ 
            board[i][j] = nullptr;
        }
    }

    board[0][0] = std::make_shared<Rook>(Color::WHITE, Position(0,0));
    board[0][1] = std::make_shared<Knight>(Color::WHITE, Position(0,1));
    board[0][2] = std::make_shared<Bishop>(Color::WHITE, Position(0,2));
    board[0][3] = std::make_shared<Queen>(Color::WHITE, Position(0,3));
    board[0][4] = std::make_shared<King>(Color::WHITE, Position(0,4));
    board[0][5] = std::make_shared<Bishop>(Color::WHITE, Position(0,5));
    board[0][6] = std::make_shared<Knight>(Color::WHITE, Position(0,6));
    board[0][7] = std::make_shared<Rook>(Color::WHITE, Position(0,7));



    for (int col = 0; col < 8; col++) {
        board[1][col] = std::make_shared<Pawn>(Color::BLACK, Position(1, col));
    }

    for (int col = 0; col < 8; col++) {
        board[6][col] = std::make_shared<Pawn>(Color::WHITE, Position(6, col));
    }

    board[7][0] = std::make_shared<Rook>(Color::BLACK, Position(7,0));
    board[7][1] = std::make_shared<Knight>(Color::BLACK, Position(7,1));
    board[7][2] = std::make_shared<Bishop>(Color::BLACK, Position(7,2));
    board[7][3] = std::make_shared<Queen>(Color::BLACK, Position(7,3));
    board[7][4] = std::make_shared<King>(Color::BLACK, Position(7,4));
    board[7][5] = std::make_shared<Bishop>(Color::BLACK, Position(7,5));
    board[7][6] = std::make_shared<Knight>(Color::BLACK, Position(7,6));
    board[7][7] = std::make_shared<Rook>(Color::BLACK, Position(7,7));

}

Game::Game() : currentPlayer(Color::WHITE), gameOver(false) { 
    board.initialize();
}

void Game::start() { 
    board.initialize(); 
    currentPlayer = Color::WHITE; 
    gameOver = false; 
    std::cout << "Chess game started!\n";
}

void Game::play() { 
    board.display();
    std::cout << "Game loop - implement GUI here\n";
}

Color Game::getCurrentPlayer() const { 
    return currentPlayer;
}

void Game::switchPlayer() {
    currentPlayer = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE; 
}

// ============= PIECE CLASS IMPLEMENTATIONS =============

// Piece constructor
Piece::Piece(Color c, PieceType t, Position pos) 
    : color(c), type(t), position(pos), hasMoved(false) {}

// Piece getters
Color Piece::getColor() const { return color; }
PieceType Piece::getType() const { return type; }
Position Piece::getPosition() const { return position; }
bool Piece::hasMovedBefore() const { return hasMoved; }

// Piece setters
void Piece::setPosition(Position pos) { position = pos; }
void Piece::setHasMoved(bool moved) { hasMoved = moved; }

// ============= BOARD CLASS IMPLEMENTATIONS =============

// Board constructor
Board::Board() {
    board.resize(8, std::vector<std::shared_ptr<Piece>>(8, nullptr));
}

// Get piece at position
std::shared_ptr<Piece> Board::getPiece(Position pos) const {
    if (!pos.isValid()) return nullptr;
    return board[pos.row][pos.col];
}

// Set piece at position
void Board::setPiece(Position pos, std::shared_ptr<Piece> piece) {
    if (!pos.isValid()) return;
    board[pos.row][pos.col] = piece;
    if (piece) {
        piece->setPosition(pos);
    }
}

// Check if position is empty
bool Board::isEmpty(Position pos) const {
    return getPiece(pos) == nullptr;
}

// Check if path is clear (for sliding pieces)
bool Board::isPathClear(Position from, Position to) const {
    int rowDir = (to.row > from.row) ? 1 : (to.row < from.row) ? -1 : 0;
    int colDir = (to.col > from.col) ? 1 : (to.col < from.col) ? -1 : 0;
    
    int currentRow = from.row + rowDir;
    int currentCol = from.col + colDir;
    
    while (currentRow != to.row || currentCol != to.col) {
        if (!isEmpty(Position(currentRow, currentCol))) {
            return false;
        }
        currentRow += rowDir;
        currentCol += colDir;
    }
    
    return true;
}

// Display the board (text-based)
void Board::display() const {
    std::cout << "\n  a b c d e f g h\n";
    for (int row = 0; row < 8; row++) {
        std::cout << (8 - row) << " ";
        for (int col = 0; col < 8; col++) {
            auto piece = getPiece(Position(row, col));
            if (piece) {
                std::cout << piece->getSymbol() << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << (8 - row) << "\n";
    }
    std::cout << "  a b c d e f g h\n\n";
}

// ============= PIECE MOVE VALIDATION =============

// Pawn move validation
bool Pawn::isValidMove(Position from, Position to, Board& board) const {
    int direction = (color == Color::WHITE) ? -1 : 1;
    int startRow = (color == Color::WHITE) ? 6 : 1;
    
    // Move forward one square
    if (to.col == from.col && to.row == from.row + direction) {
        return board.isEmpty(to);
    }
    
    // Move forward two squares from starting position
    if (to.col == from.col && from.row == startRow && to.row == from.row + 2 * direction) {
        return board.isEmpty(to) && board.isEmpty(Position(from.row + direction, from.col));
    }
    
    // Capture diagonally
    if (abs(to.col - from.col) == 1 && to.row == from.row + direction) {
        auto targetPiece = board.getPiece(to);
        return targetPiece != nullptr && targetPiece->getColor() != color;
    }
    
    return false;
}

// Rook move validation
bool Rook::isValidMove(Position from, Position to, Board& board) const {
    // Must move in straight line (same row or column)
    if (from.row != to.row && from.col != to.col) return false;
    
    // Check if path is clear
    if (!board.isPathClear(from, to)) return false;
    
    // Check destination
    auto targetPiece = board.getPiece(to);
    return targetPiece == nullptr || targetPiece->getColor() != color;
}

// Knight move validation
bool Knight::isValidMove(Position from, Position to, Board& board) const {
    int rowDiff = abs(to.row - from.row);
    int colDiff = abs(to.col - from.col);
    
    // L-shape: 2 squares in one direction, 1 in the other
    if (!((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2))) {
        return false;
    }
    
    // Check destination
    auto targetPiece = board.getPiece(to);
    return targetPiece == nullptr || targetPiece->getColor() != color;
}

// Bishop move validation
bool Bishop::isValidMove(Position from, Position to, Board& board) const {
    // Must move diagonally
    if (abs(to.row - from.row) != abs(to.col - from.col)) return false;
    
    // Check if path is clear
    if (!board.isPathClear(from, to)) return false;
    
    // Check destination
    auto targetPiece = board.getPiece(to);
    return targetPiece == nullptr || targetPiece->getColor() != color;
}

// Queen move validation (combines rook and bishop)
bool Queen::isValidMove(Position from, Position to, Board& board) const {
    // Must move in straight line or diagonally
    bool straightLine = (from.row == to.row || from.col == to.col);
    bool diagonal = (abs(to.row - from.row) == abs(to.col - from.col));
    
    if (!straightLine && !diagonal) return false;
    
    // Check if path is clear
    if (!board.isPathClear(from, to)) return false;
    
    // Check destination
    auto targetPiece = board.getPiece(to);
    return targetPiece == nullptr || targetPiece->getColor() != color;
}

// King move validation
bool King::isValidMove(Position from, Position to, Board& board) const {
    int rowDiff = abs(to.row - from.row);
    int colDiff = abs(to.col - from.col);
    
    // Can only move one square in any direction
    if (rowDiff > 1 || colDiff > 1) return false;
    
    // Check destination
    auto targetPiece = board.getPiece(to);
    return targetPiece == nullptr || targetPiece->getColor() != color;
}

// ============= GAME CLASS IMPLEMENTATIONS =============

// Make a move
bool Game::makeMove(Position from, Position to) {
    auto piece = board.getPiece(from);
    
    // Check if there's a piece at source
    if (!piece) return false;
    
    // Check if it's the correct player's turn
    if (piece->getColor() != currentPlayer) return false;
    
    // Check if move is valid
    if (!piece->isValidMove(from, to, board)) return false;
    
    // Capture piece if present
    auto capturedPiece = board.getPiece(to);
    
    // Make the move
    board.setPiece(to, piece);
    board.setPiece(from, nullptr);
    piece->setHasMoved(true);
    
    // Record move in history
    Move move(from, to);
    move.capturedPiece = capturedPiece;
    moveHistory.push_back(move);
    
    // Switch player
    switchPlayer();
    
    return true;
}

// Validate move (simplified version)
bool Game::isValidMove(Position from, Position to) const {
    auto piece = board.getPiece(from);
    if (!piece) return false;
    if (piece->getColor() != currentPlayer) return false;
    return piece->isValidMove(from, to, const_cast<Board&>(board));
}

// Check if king is in check (simplified - returns false for now)
bool Game::isInCheck(Color color) const {
    // TODO: Implement check detection
    return false;
}

// Check if checkmate (simplified - returns false for now)
bool Game::isCheckmate(Color color) const {
    // TODO: Implement checkmate detection
    return false;
}

// Check if stalemate (simplified - returns false for now)
bool Game::isStalemate(Color color) const {
    // TODO: Implement stalemate detection
    return false;
}

// Undo last move
void Game::undoMove() {
    if (moveHistory.empty()) return;
    
    Move lastMove = moveHistory.back();
    moveHistory.pop_back();
    
    auto piece = board.getPiece(lastMove.to);
    board.setPiece(lastMove.from, piece);
    board.setPiece(lastMove.to, lastMove.capturedPiece);
    
    switchPlayer();
}

int main() {
    ChessGUI gui;
    gui.run();
    return 0;
}