#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <ctime>
#include <iostream>
#include <vector>
#include <array>

#include "jugada.hpp"
#include "user.hpp"
#include "gameinfo.hpp"

enum Resultado { //dividir entre 2 en la fórmula
    BLANCAS = 2,
    TABLAS = 1,
    NEGRAS = 0
};

class Partida : public sf::Drawable
{
    // private data member
private:
    User* usuario_blancas;
    User* usuario_negras;
    std::vector<Pieza*> whitePieces;
    std::vector<Pieza*> blackPieces;
    std::vector<Pieza*> whitePiecesKilled;
    std::vector<Pieza*> blackPiecesKilled;
    std::vector<Jugada*> jugadas;
    GameInfo* gameInfo;
    time_t fecha;
    std::array<sf::RectangleShape, 64> m_boardSquares;
    std::vector<sf::RectangleShape> possibleMovesSquares;
    
    Pieza* potentiallyPieceEnPassant;
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
    Pieza* selectedPiece;
    bool turn; //true white false black
    bool orientation; // true white false black
    bool whiteCanShortCastling;
    bool blackCanShortCastling;
    bool whiteCanLongCastling;
    bool blackCanLongCastling;
    // public member function
public:
    Partida(User* usuario_blancas, User* usuario_negras, sf::Font* font);
    Pieza* tablero[64];
    // getters
    bool isJaque();
    bool isJaqueMate();
    bool getOrientation();
    Pieza* getPiezaByPos(std::string pos);
    std::vector<Jugada*> getJugadas();
    //Pieza* getPotentiallyPieceEnPassant();
    Pieza* getSelectedPiece();

    // setters
    void setResultado(Resultado r); // aplica los cambios a los jugadores
    void setOrientation(bool orientation);
    //void setPotentiallyPieceEnPassant(Pieza *p);

    // methods
    void load(sf::Color col1 = sf::Color::White, sf::Color col2 = sf::Color::Black);
    void loadFen(std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    std::string saveFen();
    std::string savePgn();
    bool aplicarJugada(Jugada* j, std::vector<int> movements); // aplica los cambios al tablero
    void undoPlay(int nPlay);
    bool applyPlay(int nPlay);
    std::vector<int> selectPiece(int pos);
    bool moveSelected(int pos, std::vector<int> validMovements);
    bool rotateBoard();
    bool isChecking(Pieza *p);
    bool checkIfChecks();
    bool checkIfCheckmate();
    bool canPieceMove(Pieza *p);
    std::vector<int> createMovesSquares();
    std::vector<int> filterValidMovements(Pieza *p);
    std::vector<int> filterIllegalMoves(Pieza *p, std::vector<int> filteredMovements);
};