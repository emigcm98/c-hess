#include <cstdlib>

#include "algorithm.hpp"

// BASE

ChessAlgorithm::ChessAlgorithm(ChessGame *chessgame, bool color)
{
    this->chessgame = chessgame;
    this->color = color;
    this->whitePieces = chessgame->getWhitePieces();
    this->blackPieces = chessgame->getBlackPieces();
}

// RANDOM CHESS ALGORITHM

RandomChessAlgorithm::RandomChessAlgorithm(ChessGame *p, bool color) : ChessAlgorithm(p, color)
{

}

float RandomChessAlgorithm::evaluatePosition()
{
    return 0;
}

int RandomChessAlgorithm::getBestOption()
{
    srand(time(0));
    // choose piece
    std::vector<int> movements;
    if (color){
        while (true){
            int n = rand() % whitePieces->size();
            Piece *p = whitePieces->at(n);
            movements = chessgame->selectPiece(p->getPos());
            if (!movements.empty()){
                break;
            }
        }
    }
    else {
        while(true){
            int n = rand() % blackPieces->size();
            Piece *p = blackPieces->at(n);
            movements = chessgame->selectPiece(p->getPos());
            if (!movements.empty()){
                break;
            }
        }
    }
 
    // select movement
    return (movements.at(rand() % movements.size()));
} 

// BASIC CHESS ALGORITHM

BasicChessAlgorithm::BasicChessAlgorithm(ChessGame *p, bool color) : ChessAlgorithm(p, color)
{
    this->prevPositionEvaluation = 0.0;
}

float BasicChessAlgorithm::evaluatePosition()
{
    // must have the higher evaluation
    
    float evaluation = 0.0;

    for (auto const &i : *whitePieces){
        if (instanceof<Pawn>(i)){
            evaluation += 1;
        }
        else if (instanceof<Bishop>(i)){
            evaluation += 3;
        }
        else if (instanceof<Knight>(i)){
            evaluation += 3;
        }
        else if (instanceof<Rook>(i)){
            evaluation += 5;
        }
        else if (instanceof<Queen>(i)){
            evaluation += 10;
        }
    }

    for (auto const &i : *blackPieces){
        if (instanceof<Pawn>(i)){
            evaluation -= 1;
        }
        else if (instanceof<Bishop>(i)){
            evaluation -= 3;
        }
        else if (instanceof<Knight>(i)){
            evaluation -= 3;
        }
        else if (instanceof<Rook>(i)){
            evaluation -= 5;
        }
        else if (instanceof<Queen>(i)){
            evaluation -= 10;
        }
    }

    return evaluation;
}

int BasicChessAlgorithm::getBestOption()
{
    int bestMovement = -1;
    Piece * piece = nullptr;

    float bestEvaluation;
    if (color) {
        bestEvaluation = -1000.0;
    }
    else {
        bestEvaluation = 1000.0;
    }

    if (color){
        for (auto const &i : *whitePieces){
            // we select
            std::vector <int> movements = chessgame->selectPiece(i->getPos());
            chessgame->deselectPiece();
            if (movements.empty()) continue;
            for (auto const &mov : movements){
                Move *j = new Move(i, mov);
                Move *j2;
                bool valid = chessgame->aplicarMove(j, movements);
                if (valid){
                    chessgame->getMoves().push_back(j);
                }
                // else {
                //     delete j;
                // }

                for (auto const &i2 : *blackPieces){
                    std::vector <int> movements2 = chessgame->selectPiece(i2->getPos());
                    chessgame->deselectPiece();
                    if (movements2.empty()) continue;
                    for (auto const &mov2 : movements2){
                        j2 = new Move(i2, mov2);
                        bool valid2 = chessgame->aplicarMove(j2, movements);
                        if (valid2){
                            chessgame->getMoves().push_back(j2);
                        }
                    }
                }

                float evaluation = evaluatePosition();
                if (evaluation > bestEvaluation){
                    bestEvaluation = evaluation;
                    bestMovement = mov;
                    piece = i;
                }

                // delete j and j2
                chessgame->undoPlay();
                chessgame->undoPlay();
                chessgame->getMoves().pop_back();
                chessgame->getMoves().pop_back();
                //delete j2;
                //delete j;
            }
        }    
    }
    else
    {
        for (auto const &i : *blackPieces){
            // we select
            std::vector <int> movements = chessgame->selectPiece(i->getPos());
            chessgame->deselectPiece();
            if (movements.empty()) continue;
            for (auto const &mov : movements){
                Move *j = new Move(i, mov);
                Move *j2;
                bool valid = chessgame->aplicarMove(j, movements);
                if (valid){
                    chessgame->getMoves().push_back(j);
                }
                // else {
                //     delete j;
                // }

                for (auto const &i2 : *whitePieces){
                    std::vector <int> movements2 = chessgame->selectPiece(i2->getPos());
                    chessgame->deselectPiece();
                    if (movements2.empty()) continue;
                    for (auto const &mov2 : movements2){
                        j2 = new Move(i2, mov2);
                        bool valid2 = chessgame->aplicarMove(j2, movements);
                        if (valid2){
                            chessgame->getMoves().push_back(j2);
                        }
                    }
                }

                float evaluation = evaluatePosition();
                if (evaluation > bestEvaluation){
                    bestEvaluation = evaluation;
                    bestMovement = mov;
                    piece = i;
                }

                // delete j and j2
                chessgame->undoPlay();
                chessgame->undoPlay();
                chessgame->getMoves().pop_back();
                chessgame->getMoves().pop_back();
                //delete j2;
                //delete j;
            }
        }
    }

    chessgame->selectPiece(piece->getPos());
    return bestMovement;
}

