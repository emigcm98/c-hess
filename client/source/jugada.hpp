#pragma once

#include "pieza.hpp"
//#include "partida.hpp"

class Jugada {

private:
    Pieza* pieza;
    int prevPos;
    int newPos;
    std::string newPosStr;
    //Partida* p;
public:
    Jugada(Pieza* pieza, std::string new_pos);
    Jugada(Pieza* pieza, int new_pos);
    bool shortCastling;
    bool longCastling;
    bool firstPieceMove;
    bool enPassant;
    // getters
    Pieza* getPieza();
    int getNewPos();
    // setters
    // methods
    bool check_movement(); // true if possible, false if not
    bool isJaque();
    bool isJaqueMate();
    std::string to_string();
};