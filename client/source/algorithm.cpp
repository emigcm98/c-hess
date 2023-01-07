#include <cstdlib>

#include "algorithm.hpp"

// BASE

ChessAlgorithm::ChessAlgorithm(ChessGame *chessgame, bool color)
{
    this->chessgame = chessgame;
    this->color = color;
    this->whitePieces = chessgame->getWhitePieces();
    this->blackPieces = chessgame->getBlackPieces();
    this->gameMoves = chessgame->getMoves();
    
    // struct tm birthdate;
    // this->computer = new User("¿¿??", "", birthdate, 0);
}

ChessAlgorithm::~ChessAlgorithm()
{
}

// RANDOM CHESS ALGORITHM

RandomChessAlgorithm::RandomChessAlgorithm(ChessGame *p, bool color) : ChessAlgorithm(p, color)
{
    struct tm birthdate;
    this->computer = new User("randomAlg", "", birthdate, 500);
}

RandomChessAlgorithm::~RandomChessAlgorithm()
{
    //std::cout << "deleting RandomChessAlgorithm" << std::endl;
}

User *ChessAlgorithm::getUser()
{
    return this->computer;
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
    if (color)
    {
        while (true)
        {
            int n = rand() % whitePieces->size();
            Piece *p = whitePieces->at(n);
            movements = chessgame->selectPiece(p->getPos());
            if (!movements.empty())
            {
                break;
            }
        }
    }
    else
    {
        while (true)
        {
            int n = rand() % blackPieces->size();
            Piece *p = blackPieces->at(n);
            movements = chessgame->selectPiece(p->getPos());
            if (!movements.empty())
            {
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

BasicChessAlgorithm::~BasicChessAlgorithm()
{
    //std::cout << "deleting BasicChessAlgorithm" << std::endl;
}

float BasicChessAlgorithm::evaluatePosition()
{
    // must have the higher evaluation

    float evaluation = 0.0;

    for (auto const &i : *whitePieces)
    {
        if (instanceof <Pawn>(i))
        {
            evaluation += 1;
        }
        else if (instanceof <Bishop>(i))
        {
            evaluation += 3;
        }
        else if (instanceof <Knight>(i))
        {
            evaluation += 3;
        }
        else if (instanceof <Rook>(i))
        {
            evaluation += 5;
        }
        else if (instanceof <Queen>(i))
        {
            evaluation += 10;
        }
    }

    for (auto const &i : *blackPieces)
    {
        if (instanceof <Pawn>(i))
        {
            evaluation -= 1;
        }
        else if (instanceof <Bishop>(i))
        {
            evaluation -= 3;
        }
        else if (instanceof <Knight>(i))
        {
            evaluation -= 3;
        }
        else if (instanceof <Rook>(i))
        {
            evaluation -= 5;
        }
        else if (instanceof <Queen>(i))
        {
            evaluation -= 10;
        }
    }

    return evaluation;
}

int BasicChessAlgorithm::getBestOption()
{
    int bestMovement = -1;
    Piece *piece = nullptr;
    float bestEvaluation;

    if (color)
    {
        bestEvaluation = -1000.0;
    }
    else
    {
        bestEvaluation = 1000.0;
        for (auto const &i : *blackPieces)
        {
            // we select
            std::vector<int> movements = chessgame->selectPiece(i->getPos());
            if (movements.empty()){
                chessgame->deselectPiece();
                continue;
            }
            for (auto const &mov : movements)
            {
                Move *j = nullptr;
                Move *j2 = nullptr;
                //bool valid = chessgame->applyMove(j, movements);
                std::cout << "1. Testing " << i->getNameFEN() << " " << toChessPosition(mov) << std::endl;
                j = chessgame->moveSelected(mov);

                if (j == nullptr){
                    continue;
                }
                //chessgame->passTurn();

                for (auto const &i2 : *whitePieces)
                {
                    std::vector<int> movements2 = chessgame->selectPiece(i2->getPos());
                    if (movements2.empty())
                    {
                        chessgame->deselectPiece();
                        continue;
                    }
                    for (auto const &mov2 : movements2)
                    {
                        j2 = chessgame->moveSelected(mov2);
                        //std::cout << "2. Testing " << i2->getNameFEN() << " " << toChessPosition(mov2) << std::endl;

                        if (j2 == nullptr)
                        {
                            continue;
                        }

                        float evaluation = evaluatePosition();
                        //std::cout << ". Ev: " << evaluation << std::endl;
                        if (evaluation < bestEvaluation)
                        {
                            std::cout << "new best evaluation: " << evaluation << " < " << bestEvaluation << ". Best: " << i->getNameFEN() << " " << toChessPosition(i->getPos()) << std::endl;

                            bestEvaluation = evaluation;
                            bestMovement = mov;
                            piece = i;
                        }
                        // delete j2
                        chessgame->undoMove();
                        delete j2;
                        
                    }
                }

                // delete j
                chessgame->undoMove();
                delete j;
            }
        }
    }
    
    std::cout << "mov chosen: " << piece->getNameFEN() << " " << toChessPosition(bestMovement) << std::endl;
    //chessgame->passTurn();
    chessgame->selectPiece(piece->getPos());
    return bestMovement;
}
