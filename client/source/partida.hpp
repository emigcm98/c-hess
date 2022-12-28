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
    WHITE = 2,
    DRAW = 1,
    BLACK = 0
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

    std::vector<int> validMovements;
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
    Pieza* selectedPiece;
    bool turn; //true white false black
    bool orientation; // true white false black
    bool whiteCanShortCastling;
    bool blackCanShortCastling;
    bool whiteCanLongCastling;
    bool blackCanLongCastling;

    bool finished;
    Resultado r;
public:
    Partida(User* usuario_blancas, User* usuario_negras, sf::Font* font);
    Pieza* tablero[64];
    // getters
    bool getOrientation();
    Pieza* getPiezaByPos(std::string pos);
    std::vector<Jugada*> getJugadas();
    Pieza* getSelectedPiece();
    bool isFinished();

    std::vector<Pieza*>* getWhitePieces();
    std::vector<Pieza*>* getBlackPieces();

    // setters
    void setResultado(Resultado r); // aplica los cambios a los jugadores
    void setOrientation(bool orientation);

    // methods
    void load(sf::Color col1 = sf::Color::White, sf::Color col2 = sf::Color::Black);
    void loadFen(std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w");
    std::string saveFen();
    std::string savePgn();
    bool aplicarJugada(Jugada* j, std::vector<int> movements); // aplica los cambios al tablero
    void undoPlay(int nPlay = -1);
    bool applyPlay(int nPlay);
    void deselectPiece();
    std::vector<int> selectPiece(int pos);
    bool moveSelected(int pos);
    //bool fakeMove(int pos, std::vector<int> validMovements);
    bool rotateBoard();
    bool isChecking(Pieza *p);
    bool checkIfChecks(bool color);
    bool checkIfCheckmate(bool color);
    bool checkIfDrawsByMaterial();
    bool checkIfDrawsByPosition(bool color);
    bool canPieceMove(Pieza *p);
    Pieza* promote(Pieza *p);
    std::vector<int> createMovesSquares();
    std::vector<int> filterValidMovements(Pieza *p);
    std::vector<int> filterIllegalMoves(Pieza *p, std::vector<int> filteredMovements, bool color);

    void playNextMove();
};