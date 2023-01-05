#include "chessgame.hpp"

class ChessAlgorithm
{
    protected:
        bool color;
        std::vector<Piece *> *whitePieces;
        std::vector<Piece *> *blackPieces;
        std::vector<Move *> *gameMoves; 
        ChessGame *chessgame;
        User* computer;
        //Piece* tablero[64];
        float prevPositionEvaluation;
    public:
        ChessAlgorithm(ChessGame *chessgame, bool color);
        User* getUser();
        virtual float evaluatePosition() = 0; // with fake moveSelected() to evaluate best position
        virtual int getBestOption() = 0;
};

class RandomChessAlgorithm : public ChessAlgorithm
{
    public: 
        RandomChessAlgorithm(ChessGame *chessgame, bool color);
        virtual float evaluatePosition(); // with fake moveSelected() to evaluate best position
        virtual int getBestOption();
};

class BasicChessAlgorithm : public ChessAlgorithm
{
    private:
        int pawnValue = 1;
        int bishopValue = 3;
        int knightValue = 3;
        int rookValue = 5;
        int queenValue = 10;
    public:
        BasicChessAlgorithm(ChessGame *chessgame, bool color);
        virtual float evaluatePosition(); // with fake moveSelected() to evaluate best position
        virtual int getBestOption();
};