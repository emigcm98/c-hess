#include "algorithm.hpp"
#include "user.hpp"
#include "piece.hpp"
#include "configuration.cpp"


class ChessGameHandler{

private:
    ChessGame* chessgame;
    ChessAlgorithm* ca;
    User* player;
    bool humanColor;
    bool orientation;
    
    int nMove;
    std::vector<Move *> *gameMoves;
    Piece *selectedPiece;

    bool turn;

    sf::Font font;
    sf::RenderWindow window;

public:
    ChessGameHandler(User *player, bool humanColor);
    int start();
};