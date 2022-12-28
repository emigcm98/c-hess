#pragma once

#include <ctype.h>
#include "piece.hpp"

class Move {

private:
    Piece* piece;
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
    Piece* eatenPiece;

    std::string str;
public:
    Move(Piece* p, std::string newPos);
    Move(Piece* p, int newPos);
    
    std::vector<Piece*> checkingPieces;

    // getters
    Piece* getPiece();
    int getNewPos();
    int getPrevPos();
    bool isShortCastling();
    bool isLongCastling();
    bool isFirstPieceMove();
    Piece *getEatenPiece();
    // setters
    void setPiece(Piece *p);
    void setCheck(bool check);
    void setCheckmate(bool checkmate);
    void setPromoted(bool promoted);
    void setFirstPieceMoved(bool moved);
    // methods
    void eat(Piece *p);
    bool isCheck();
    bool isCheckmate();
    void shortCastle();
    void longCastle();

    void generateString();
    std::string to_string();
};