#include <iostream> 
#include <SMFL/Graphics.hpp> 

const int SQUARE_SIZE = 80; 

const int BOARD_SIZE = 8;
const int WINDOW_SIZE = SQUARE_SIZE * BOARD_SIZE

class ChessGUI { 
    private: 
    sf::RenderWindow window; 
    sf::Font font; 

    sf::Color lightSquare; 
    sf::Color darkSquare; 
    
    bool pieceSelected; 
    int selectedRow; 
    int selectedCol; 

    public: 
        ChessGUI() : window(s)
}