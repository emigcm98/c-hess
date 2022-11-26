#pragma once

#include "pieza.hpp"
//#include "partida.hpp"

class Jugada {

private:
    Pieza* pieza;
    int prev_file;
    int prev_col;
    int new_file;
    int new_col;
    //Partida* p;
public:
    Jugada(Pieza* pieza, int new_file, int new_col);
    // getters
    Pieza* getPieza();
    int getNewFile();
    char getNewFile_text();
    int getNewCol();
    // setters
    // methods
    bool check_movement(); // true if possible, false if not
};