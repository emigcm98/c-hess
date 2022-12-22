#pragma once

#include <ctype.h>
#include "pieza.hpp"
//#include "partida.hpp"

class Jugada {

private:
    Pieza* pieza;
    int prevPos;
    int newPos;
    std::string newPosStr;
    bool shortCastling;
    bool longCastling;
    bool firstPieceMove;
    bool enPassant;
    bool check;
    bool checkmate;
    Pieza* eatenPiece;

    std::string str;
public:
    Jugada(Pieza* pieza, std::string new_pos);
    Jugada(Pieza* pieza, int new_pos);
    
    std::vector<Pieza*> checkingPieces;

    // getters
    Pieza* getPieza();
    int getNewPos();
    int getPrevPos();
    bool isShortCastling();
    bool isLongCastling();
    bool isFirstPieceMove();
    Pieza *getEatenPiece();
    // setters
    void setJaque(bool check);
    void setJaqueMate(bool checkmate);
    void setFirstPieceMoved(bool moved);
    // methods
    void eat(Pieza *p);
    bool isJaque();
    bool isJaqueMate();
    void shortCastle();
    void longCastle();

    void generateString();
    std::string to_string();
};