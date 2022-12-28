#include "partida.hpp"

class ChessAlgorithm
{
    protected:
        bool color;
        std::vector<Pieza *> *whitePieces;
        std::vector<Pieza *> *blackPieces;
        Partida *partida;
        //Pieza* tablero[64];
        float prevPositionEvaluation;
    public:
        ChessAlgorithm(Partida *p, bool color);
        virtual float evaluatePosition() = 0; // with fake moveSelected() to evaluate best position
        virtual int getBestOption() = 0;
};

class RandomChessAlgorithm : public ChessAlgorithm
{
    public: 
        RandomChessAlgorithm(Partida *p, bool color);
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
        BasicChessAlgorithm(Partida *p, bool color);
        virtual float evaluatePosition(); // with fake moveSelected() to evaluate best position
        virtual int getBestOption();
};