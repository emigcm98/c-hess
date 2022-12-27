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
    char pieceNameNotation;

    bool shortCastling;
    bool longCastling;
    bool firstPieceMove;
    bool enPassant;
    bool check;
    bool checkmate;
    bool promoted;
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
    void setPiece(Pieza *p);
    void setCheck(bool check);
    void setCheckmate(bool checkmate);
    void setPromoted(bool promoted);
    void setFirstPieceMoved(bool moved);
    // methods
    void eat(Pieza *p);
    bool isCheck();
    bool isCheckmate();
    void shortCastle();
    void longCastle();

    void generateString();
    std::string to_string();
};