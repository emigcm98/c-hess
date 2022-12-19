#pragma once

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
    //Partida* p;
public:
    Jugada(Pieza* pieza, std::string new_pos);
    Jugada(Pieza* pieza, int new_pos);
    // getters
    Pieza* getPieza();
    int getNewPos();
    bool isShortCastling();
    bool isLongCastling();
    bool isFirstPieceMove();
    // setters
    // methods
    bool check_movement(); // true if possible, false if not
    bool isJaque();
    bool isJaqueMate();
    void shortCastle();
    void longCastle();
    void firstPieceMoved();
    std::string to_string();
};