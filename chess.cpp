#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
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
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};

// Forward declarations
class Game;
class Piece;
class Board;

enum class GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

class ChessGUI {
    private:
    sf::RenderWindow window;
    sf::Font font;

    sf::Color lightSquare;
    sf::Color darkSquare;

    Game* game;

    bool pieceSelected;
    Position selectedPos;
    std::vector<Position> validMoves;

    GameState state;
    Color playerColor;
    Color aiColor;
    bool isAITurn;

    public:
        ChessGUI() : window(sf::VideoMode({WINDOW_SIZE, WINDOW_SIZE + 100}), "Chess Game"),
        lightSquare(240, 217, 181),
        darkSquare(181, 136, 99),
        game(nullptr),
        pieceSelected(false),
        selectedPos(-1, -1),
        state(GameState::MENU),
        playerColor(Color::WHITE),
        aiColor(Color::BLACK),
        isAITurn(false) {
            if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
                std::cerr << "Failed to load font\n";
            }
        }

        void setGame(Game* g);
        void run();
        void setPlayerColor(Color color);

private:
    void handleEvents();
    void handleMenuClick(int x, int y);
    void handleMouseClick(int x, int y);
    void calculateValidMoves();
    void makeAIMove();

    void render();
    void drawMenu();
    void drawBoard();
    void drawPieces();
    void drawStatusBar();
    std::string getPieceUnicode(PieceType type, Color color);
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

    // Get piece at position
    std::shared_ptr<Piece> getPieceAt(Position pos) const;

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

std::shared_ptr<Piece> Game::getPieceAt(Position pos) const {
    return board.getPiece(pos);
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

// ============= CHESS GUI IMPLEMENTATIONS =============

void ChessGUI::setGame(Game* g) {
    game = g;
}

void ChessGUI::setPlayerColor(Color color) {
    playerColor = color;
    aiColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
}

void ChessGUI::run() {
    while (window.isOpen()) {
        handleEvents();

        // Make AI move if it's AI's turn
        if (state == GameState::PLAYING && game && game->getCurrentPlayer() == aiColor && !isAITurn) {
            isAITurn = true;
            makeAIMove();
            isAITurn = false;
        }

        render();
    }
}

void ChessGUI::handleEvents() {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                if (state == GameState::MENU) {
                    handleMenuClick(mousePressed->position.x, mousePressed->position.y);
                } else if (state == GameState::PLAYING) {
                    handleMouseClick(mousePressed->position.x, mousePressed->position.y);
                }
            }
        }
    }
}

void ChessGUI::handleMenuClick(int x, int y) {
    // White button: x from 100-300, y from 250-330
    if (x >= 100 && x <= 300 && y >= 250 && y <= 330) {
        setPlayerColor(Color::WHITE);
        state = GameState::PLAYING;
        std::cout << "Player chose WHITE\n";
    }
    // Black button: x from 340-540, y from 250-330
    else if (x >= 340 && x <= 540 && y >= 250 && y <= 330) {
        setPlayerColor(Color::BLACK);
        state = GameState::PLAYING;
        isAITurn = true; // AI (white) goes first
        std::cout << "Player chose BLACK\n";
    }
    // Exit button: x from 220-420, y from 380-460
    else if (x >= 220 && x <= 420 && y >= 380 && y <= 460) {
        window.close();
        std::cout << "Game closed\n";
    }
}

void ChessGUI::handleMouseClick(int x, int y) {
    if (!game) return;

    // Check if exit button was clicked (in status bar area)
    if (y >= WINDOW_SIZE) {
        if (x >= WINDOW_SIZE - 140 && x <= WINDOW_SIZE - 20 && y >= WINDOW_SIZE + 25 && y <= WINDOW_SIZE + 75) {
            window.close();
            std::cout << "Game closed\n";
            return;
        }
    }

    // Only allow player to move on their turn
    if (game->getCurrentPlayer() != playerColor) return;

    int col = x / SQUARE_SIZE;
    int row = y / SQUARE_SIZE;
    Position clickedPos(row, col);

    if (!clickedPos.isValid()) return;

    if (!pieceSelected) {
        // Try to select a piece
        auto piece = game->getPieceAt(clickedPos);
        if (piece && piece->getColor() == playerColor) {
            pieceSelected = true;
            selectedPos = clickedPos;
            calculateValidMoves();
            std::cout << "Selected piece at (" << row << ", " << col << ")\n";
        }
    } else {
        // Try to move the selected piece
        if (game->makeMove(selectedPos, clickedPos)) {
            std::cout << "Moved piece from (" << selectedPos.row << ", " << selectedPos.col
                     << ") to (" << clickedPos.row << ", " << clickedPos.col << ")\n";
        } else {
            std::cout << "Invalid move\n";
        }
        pieceSelected = false;
        validMoves.clear();
    }
}

void ChessGUI::calculateValidMoves() {
    validMoves.clear();
    if (!game) return;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Position testPos(row, col);
            if (game->isValidMove(selectedPos, testPos)) {
                validMoves.push_back(testPos);
            }
        }
    }
}

void ChessGUI::makeAIMove() {
    if (!game) return;

    // Helper function to get piece value
    auto getPieceValue = [](PieceType type) -> int {
        switch (type) {
            case PieceType::PAWN: return 10;
            case PieceType::KNIGHT: return 30;
            case PieceType::BISHOP: return 30;
            case PieceType::ROOK: return 50;
            case PieceType::QUEEN: return 90;
            case PieceType::KING: return 900;
            default: return 0;
        }
    };

    // Helper function to evaluate position control (center control bonus)
    auto getPositionValue = [](Position pos) -> int {
        int centerBonus = 0;
        // Reward center control
        if ((pos.row >= 3 && pos.row <= 4) && (pos.col >= 3 && pos.col <= 4)) {
            centerBonus = 5;
        } else if ((pos.row >= 2 && pos.row <= 5) && (pos.col >= 2 && pos.col <= 5)) {
            centerBonus = 2;
        }
        return centerBonus;
    };

    // Advanced AI: evaluate moves and pick the best one
    std::vector<std::pair<Position, Position>> possibleMoves;
    std::vector<int> moveScores;

    for (int fromRow = 0; fromRow < 8; fromRow++) {
        for (int fromCol = 0; fromCol < 8; fromCol++) {
            Position from(fromRow, fromCol);
            auto piece = game->getPieceAt(from);

            if (piece && piece->getColor() == aiColor) {
                for (int toRow = 0; toRow < 8; toRow++) {
                    for (int toCol = 0; toCol < 8; toCol++) {
                        Position to(toRow, toCol);
                        if (game->isValidMove(from, to)) {
                            possibleMoves.push_back({from, to});

                            // Calculate move score
                            int score = 0;

                            // Check if move captures an enemy piece
                            auto targetPiece = game->getPieceAt(to);
                            if (targetPiece && targetPiece->getColor() != aiColor) {
                                score += getPieceValue(targetPiece->getType());
                            }

                            // Add position value bonus
                            score += getPositionValue(to);

                            // Bonus for developing pieces (moving from starting row)
                            int startRow = (aiColor == Color::WHITE) ? 0 : 7;
                            if (from.row == startRow) {
                                score += 3;
                            }

                            // Pawn advancement bonus
                            if (piece->getType() == PieceType::PAWN) {
                                int direction = (aiColor == Color::WHITE) ? 1 : -1;
                                int advancement = (to.row - from.row) * direction;
                                score += advancement * 2;
                            }

                            moveScores.push_back(score);
                        }
                    }
                }
            }
        }
    }

    if (!possibleMoves.empty()) {
        // Find the best move(s)
        int maxScore = *std::max_element(moveScores.begin(), moveScores.end());

        // Collect all moves with the maximum score
        std::vector<std::pair<Position, Position>> bestMoves;
        for (size_t i = 0; i < possibleMoves.size(); i++) {
            if (moveScores[i] == maxScore) {
                bestMoves.push_back(possibleMoves[i]);
            }
        }

        // Pick randomly among the best moves
        int randomIndex = rand() % bestMoves.size();
        auto move = bestMoves[randomIndex];

        game->makeMove(move.first, move.second);
        std::cout << "AI moved from (" << move.first.row << ", " << move.first.col
                 << ") to (" << move.second.row << ", " << move.second.col << ")"
                 << " [Score: " << maxScore << "]\n";
    }
}

void ChessGUI::render() {
    window.clear();

    if (state == GameState::MENU) {
        drawMenu();
    } else if (state == GameState::PLAYING) {
        drawBoard();
        drawPieces();
        drawStatusBar();
    }

    window.display();
}

void ChessGUI::drawMenu() {
    // Draw title
    sf::Text title(font);
    title.setString("Chess Game");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(sf::Vector2f(
        (WINDOW_SIZE - titleBounds.size.x) / 2 - titleBounds.position.x,
        50
    ));
    window.draw(title);

    // Draw instruction
    sf::Text instruction(font);
    instruction.setString("Choose Your Color:");
    instruction.setCharacterSize(30);
    instruction.setFillColor(sf::Color::White);
    sf::FloatRect instructionBounds = instruction.getLocalBounds();
    instruction.setPosition(sf::Vector2f(
        (WINDOW_SIZE - instructionBounds.size.x) / 2 - instructionBounds.position.x,
        150
    ));
    window.draw(instruction);

    // Draw White button
    sf::RectangleShape whiteButton(sf::Vector2f(200, 80));
    whiteButton.setPosition(sf::Vector2f(100, 250));
    whiteButton.setFillColor(sf::Color(240, 240, 240));
    whiteButton.setOutlineColor(sf::Color::Black);
    whiteButton.setOutlineThickness(3);
    window.draw(whiteButton);

    sf::Text whiteText(font);
    whiteText.setString("WHITE");
    whiteText.setCharacterSize(30);
    whiteText.setFillColor(sf::Color::Black);
    sf::FloatRect whiteBounds = whiteText.getLocalBounds();
    whiteText.setPosition(sf::Vector2f(
        200 - whiteBounds.size.x / 2 - whiteBounds.position.x,
        290 - whiteBounds.size.y / 2 - whiteBounds.position.y
    ));
    window.draw(whiteText);

    // Draw Black button
    sf::RectangleShape blackButton(sf::Vector2f(200, 80));
    blackButton.setPosition(sf::Vector2f(340, 250));
    blackButton.setFillColor(sf::Color(40, 40, 40));
    blackButton.setOutlineColor(sf::Color::White);
    blackButton.setOutlineThickness(3);
    window.draw(blackButton);

    sf::Text blackText(font);
    blackText.setString("BLACK");
    blackText.setCharacterSize(30);
    blackText.setFillColor(sf::Color::White);
    sf::FloatRect blackBounds = blackText.getLocalBounds();
    blackText.setPosition(sf::Vector2f(
        440 - blackBounds.size.x / 2 - blackBounds.position.x,
        290 - blackBounds.size.y / 2 - blackBounds.position.y
    ));
    window.draw(blackText);

    // Draw Exit button
    sf::RectangleShape exitButton(sf::Vector2f(200, 80));
    exitButton.setPosition(sf::Vector2f(220, 380));
    exitButton.setFillColor(sf::Color(180, 50, 50));
    exitButton.setOutlineColor(sf::Color::White);
    exitButton.setOutlineThickness(3);
    window.draw(exitButton);

    sf::Text exitText(font);
    exitText.setString("EXIT");
    exitText.setCharacterSize(30);
    exitText.setFillColor(sf::Color::White);
    sf::FloatRect exitBounds = exitText.getLocalBounds();
    exitText.setPosition(sf::Vector2f(
        320 - exitBounds.size.x / 2 - exitBounds.position.x,
        420 - exitBounds.size.y / 2 - exitBounds.position.y
    ));
    window.draw(exitText);
}

void ChessGUI::drawBoard() {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            sf::RectangleShape square(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            square.setPosition(sf::Vector2f(col * SQUARE_SIZE, row * SQUARE_SIZE));

            Position pos(row, col);

            // Highlight selected square
            if (pieceSelected && pos.row == selectedPos.row && pos.col == selectedPos.col) {
                square.setFillColor(sf::Color(255, 255, 0, 150)); // Yellow highlight
            }
            // Highlight valid move squares
            else if (std::find(validMoves.begin(), validMoves.end(), pos) != validMoves.end()) {
                if ((row + col) % 2 == 0) {
                    square.setFillColor(sf::Color(170, 255, 170)); // Light green
                } else {
                    square.setFillColor(sf::Color(140, 200, 140)); // Dark green
                }
            }
            else if ((row + col) % 2 == 0) {
                square.setFillColor(lightSquare);
            } else {
                square.setFillColor(darkSquare);
            }

            window.draw(square);
        }
    }
}

void ChessGUI::drawPieces() {
    if (!game) return;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Position pos(row, col);
            auto piece = game->getPieceAt(pos);

            if (piece) {
                // SFML 3.0: Text constructor requires font as first parameter
                sf::Text text(font);
                text.setString(getPieceUnicode(piece->getType(), piece->getColor()));
                text.setCharacterSize(48);

                // White pieces = light color, Black pieces = dark color
                if (piece->getColor() == Color::WHITE) {
                    text.setFillColor(sf::Color(245, 245, 220)); // Beige/cream for white pieces
                    text.setOutlineColor(sf::Color::Black);
                } else {
                    text.setFillColor(sf::Color(40, 40, 40)); // Dark gray for black pieces
                    text.setOutlineColor(sf::Color::White);
                }
                text.setOutlineThickness(2);

                // Center the piece in the square
                sf::FloatRect bounds = text.getLocalBounds();
                text.setPosition(sf::Vector2f(
                    col * SQUARE_SIZE + (SQUARE_SIZE - bounds.size.x) / 2 - bounds.position.x,
                    row * SQUARE_SIZE + (SQUARE_SIZE - bounds.size.y) / 2 - bounds.position.y
                ));

                window.draw(text);
            }
        }
    }
}

void ChessGUI::drawStatusBar() {
    if (!game) return;

    // Draw status bar background
    sf::RectangleShape statusBar(sf::Vector2f(WINDOW_SIZE, 100));
    statusBar.setPosition(sf::Vector2f(0, WINDOW_SIZE));
    statusBar.setFillColor(sf::Color(50, 50, 50));
    window.draw(statusBar);

    // Display current turn
    Color currentPlayer = game->getCurrentPlayer();
    std::string turnText = "Current Turn: ";
    turnText += (currentPlayer == Color::WHITE) ? "WHITE" : "BLACK";

    sf::Text turn(font);
    turn.setString(turnText);
    turn.setCharacterSize(24);
    turn.setFillColor(sf::Color::White);
    turn.setPosition(sf::Vector2f(20, WINDOW_SIZE + 15));
    window.draw(turn);

    // Display player and AI colors
    std::string roleText = "You: ";
    roleText += (playerColor == Color::WHITE) ? "WHITE" : "BLACK";
    roleText += "  |  AI: ";
    roleText += (aiColor == Color::WHITE) ? "WHITE" : "BLACK";

    sf::Text role(font);
    role.setString(roleText);
    role.setCharacterSize(24);
    role.setFillColor(sf::Color::White);
    role.setPosition(sf::Vector2f(20, WINDOW_SIZE + 50));
    window.draw(role);

    // Draw Exit button
    sf::RectangleShape exitButton(sf::Vector2f(120, 50));
    exitButton.setPosition(sf::Vector2f(WINDOW_SIZE - 140, WINDOW_SIZE + 25));
    exitButton.setFillColor(sf::Color(180, 50, 50));
    exitButton.setOutlineColor(sf::Color::White);
    exitButton.setOutlineThickness(2);
    window.draw(exitButton);

    sf::Text exitText(font);
    exitText.setString("EXIT");
    exitText.setCharacterSize(20);
    exitText.setFillColor(sf::Color::White);
    sf::FloatRect exitBounds = exitText.getLocalBounds();
    exitText.setPosition(sf::Vector2f(
        WINDOW_SIZE - 80 - exitBounds.size.x / 2 - exitBounds.position.x,
        WINDOW_SIZE + 50 - exitBounds.size.y / 2 - exitBounds.position.y
    ));
    window.draw(exitText);
}

std::string ChessGUI::getPieceUnicode(PieceType type, Color color) {
    // Using ASCII letters for pieces (uppercase = white, lowercase = black)
    switch (type) {
        case PieceType::KING:   return (color == Color::WHITE) ? "K" : "k";
        case PieceType::QUEEN:  return (color == Color::WHITE) ? "Q" : "q";
        case PieceType::ROOK:   return (color == Color::WHITE) ? "R" : "r";
        case PieceType::BISHOP: return (color == Color::WHITE) ? "B" : "b";
        case PieceType::KNIGHT: return (color == Color::WHITE) ? "N" : "n";
        case PieceType::PAWN:   return (color == Color::WHITE) ? "P" : "p";
        default: return "";
    }
}

int main() {
    // Seed random number generator for AI
    srand(static_cast<unsigned int>(time(nullptr)));

    Game game;
    ChessGUI gui;
    gui.setGame(&game);
    gui.run();
    return 0;
}