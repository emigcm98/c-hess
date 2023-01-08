#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <ctime>
#include <iostream>
#include <vector>
#include <array>

#include "move.hpp"
#include "user.hpp"
#include "gameinfo.hpp"
#include "promotionComponent.hpp"

enum Result { //divided by 2 in formula
    WHITE = 2,
    DRAW = 1,
    BLACK = 0
};

class ChessGame : public sf::Drawable
{
    // private data member
private:
    User* whitePlayer;
    User* blackPlayer;
    std::vector<Piece*> whitePieces;
    std::vector<Piece*> blackPieces;
    std::vector<Piece*> whitePiecesKilled;
    std::vector<Piece*> blackPiecesKilled;
    std::vector<Move*> moves;

    GameInfo* gameInfo;
    PromotionComponent* promotionComponent;

    time_t fecha;
    std::array<sf::RectangleShape, 64> m_boardSquares;
    std::vector<sf::RectangleShape> possibleMovesSquares;
    
    Piece* potentiallyPieceEnPassant;

    std::vector<int> validMovements;
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
    Piece* selectedPiece;
    bool turn; //true white false black
    bool orientation; // true white false black
    bool whiteCanShortCastling;
    bool blackCanShortCastling;
    bool whiteCanLongCastling;
    bool blackCanLongCastling;
    int fiftyMoveRuleNumber;

    bool selectingPromoted;

    bool finished;
    Result r;
public:
    ChessGame(User* whitePlayer, User* blackPlayer, sf::Font* font);
    ~ChessGame();
    Piece* board[64];
    // getters
    bool getOrientation();
    Piece* getPieceByPos(std::string pos);
    std::vector<Move*>* getMoves();
    Piece* getSelectedPiece();
    bool isFinished();

    std::vector<Piece*>* getWhitePieces();
    std::vector<Piece*>* getBlackPieces();
    std::vector<Piece*>* getWhitePiecesKilled();
    std::vector<Piece*>* getBlackPiecesKilled();

    bool* getTurn();

    bool getIfSelectingPromoted();

    // setters
    void setResult(Result r); // aplica los cambios a los jugadores
    void setOrientation(bool orientation);

    // methods
    void load(sf::Color col1 = sf::Color::White, sf::Color col2 = sf::Color::Black);
    void loadFen(std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    std::string saveFen();
    std::string savePgn();
    void passTurn();
    bool applyMove(Move* j, std::vector<int> movements); // aplica los cambios al tablero
    void undoMove(int nPlay = -1);
    bool applyPlay(int nPlay);
    void deselectPiece();
    std::vector<int> selectPiece(int pos);
    Move* moveSelected(int pos, bool createPromotionComponent = true);
    //bool fakeMove(int pos, std::vector<int> validMovements);
    bool rotateBoard();
    bool isChecking(Piece *p);
    bool checkIfChecks(bool color);
    bool checkIfCheckmate(bool color);
    bool checkIfDrawsByMaterial();
    bool checkIfDrawsByPosition(bool color);
    bool canPieceMove(Piece *p);
    Move* updateStatusAfterMoving(Move *m);
    Piece* promote(Move *m, char pieceNameNotation = 'Q');
    std::vector<int> createMovesSquares();
    std::vector<int> filterValidMoves(Piece *p);
    std::vector<int> filterIllegalMoves(Piece *p, std::vector<int> filteredMovements, bool color);

    char getPieceType(int pos);
};