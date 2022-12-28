#include <cstdlib>

#include "algorithm.hpp"

// BASE

ChessAlgorithm::ChessAlgorithm(Partida *p, bool color)
{
    this->partida = p;
    this->color = color;
    this->whitePieces = p->getWhitePieces();
    this->blackPieces = p->getBlackPieces();
}

// RANDOM CHESS ALGORITHM

RandomChessAlgorithm::RandomChessAlgorithm(Partida *p, bool color) : ChessAlgorithm(p, color)
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
            Pieza *p = whitePieces->at(n);
            movements = partida->selectPiece(p->getPos());
            if (!movements.empty()){
                break;
            }
        }
    }
    else {
        while(true){
            int n = rand() % blackPieces->size();
            Pieza *p = blackPieces->at(n);
            movements = partida->selectPiece(p->getPos());
            if (!movements.empty()){
                break;
            }
        }
    }
 
    // select movement
    return (movements.at(rand() % movements.size()));
} 

// BASIC CHESS ALGORITHM

BasicChessAlgorithm::BasicChessAlgorithm(Partida *p, bool color) : ChessAlgorithm(p, color)
{
    this->prevPositionEvaluation = 0.0;
}

float BasicChessAlgorithm::evaluatePosition()
{
    // must have the higher evaluation
    
    float evaluation = 0.0;

    for (auto const &i : *whitePieces){
        if (instanceof<Peon>(i)){
            evaluation += 1;
        }
        else if (instanceof<Alfil>(i)){
            evaluation += 3;
        }
        else if (instanceof<Caballo>(i)){
            evaluation += 3;
        }
        else if (instanceof<Torre>(i)){
            evaluation += 5;
        }
        else if (instanceof<Dama>(i)){
            evaluation += 10;
        }
    }

    for (auto const &i : *blackPieces){
        if (instanceof<Peon>(i)){
            evaluation -= 1;
        }
        else if (instanceof<Alfil>(i)){
            evaluation -= 3;
        }
        else if (instanceof<Caballo>(i)){
            evaluation -= 3;
        }
        else if (instanceof<Torre>(i)){
            evaluation -= 5;
        }
        else if (instanceof<Dama>(i)){
            evaluation -= 10;
        }
    }

    return evaluation;
}

int BasicChessAlgorithm::getBestOption()
{
    int bestMovement = -1;
    Pieza * piece = nullptr;

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
            std::vector <int> movements = partida->selectPiece(i->getPos());
            partida->deselectPiece();
            if (movements.empty()) continue;
            for (auto const &mov : movements){
                Jugada *j = new Jugada(i, mov);
                Jugada *j2;
                bool valid = partida->aplicarJugada(j, movements);
                if (valid){
                    partida->getJugadas().push_back(j);
                }
                // else {
                //     delete j;
                // }

                for (auto const &i2 : *blackPieces){
                    std::vector <int> movements2 = partida->selectPiece(i2->getPos());
                    partida->deselectPiece();
                    if (movements2.empty()) continue;
                    for (auto const &mov2 : movements2){
                        j2 = new Jugada(i2, mov2);
                        bool valid2 = partida->aplicarJugada(j2, movements);
                        if (valid2){
                            partida->getJugadas().push_back(j2);
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
                partida->undoPlay();
                partida->undoPlay();
                partida->getJugadas().pop_back();
                partida->getJugadas().pop_back();
                //delete j2;
                //delete j;
            }
        }    
    }
    else
    {
        for (auto const &i : *blackPieces){
            // we select
            std::vector <int> movements = partida->selectPiece(i->getPos());
            partida->deselectPiece();
            if (movements.empty()) continue;
            for (auto const &mov : movements){
                Jugada *j = new Jugada(i, mov);
                Jugada *j2;
                bool valid = partida->aplicarJugada(j, movements);
                if (valid){
                    partida->getJugadas().push_back(j);
                }
                // else {
                //     delete j;
                // }

                for (auto const &i2 : *whitePieces){
                    std::vector <int> movements2 = partida->selectPiece(i2->getPos());
                    partida->deselectPiece();
                    if (movements2.empty()) continue;
                    for (auto const &mov2 : movements2){
                        j2 = new Jugada(i2, mov2);
                        bool valid2 = partida->aplicarJugada(j2, movements);
                        if (valid2){
                            partida->getJugadas().push_back(j2);
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
                partida->undoPlay();
                partida->undoPlay();
                partida->getJugadas().pop_back();
                partida->getJugadas().pop_back();
                //delete j2;
                //delete j;
            }
        }
    }

    partida->selectPiece(piece->getPos());
    return bestMovement;
}

