#include "jugada.hpp"
// #include "partida.hpp"

Jugada::Jugada(Pieza* pieza, int new_file, int new_col)
{
    this->pieza = pieza;
    this->prev_file = pieza->getFile();
    this->prev_col = pieza->getCol();
    this->new_file = new_file;
    this->new_col = new_col;
    //this->p = p;
}


bool Jugada::check_movement()
{
    bool is_possible = true;

    // if (is_possible == false){
    //     delete this;
    // }
    // else {
    //     p->aplicarJugada(this);
    // }

    return is_possible;   
}

Pieza* Jugada::getPieza()
{
    return this->pieza;
}

int Jugada::getNewFile()
{
    return new_file;
}

char Jugada::getNewFile_text()
{
    // 97 is 'a'
    return char(new_file+97);
}

int Jugada::getNewCol()
{
    return new_col;
}