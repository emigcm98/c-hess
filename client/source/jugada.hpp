#pragma once

#include "pieza.hpp"
//#include "partida.hpp"

class Jugada {

private:
    Pieza* pieza;
    int prev_pos;
    int new_pos;
    //Partida* p;
public:
    Jugada(Pieza* pieza, std::string new_pos);
    // getters
    Pieza* getPieza();
    int getNewPos();
    // setters
    // methods
    bool check_movement(); // true if possible, false if not
};