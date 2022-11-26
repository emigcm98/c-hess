#include "jugada.hpp"
// #include "partida.hpp"

Jugada::Jugada(Pieza* pieza, std::string new_pos)
{
    this->pieza = pieza;
    this->prev_pos = pieza->getPos();
    this->new_pos = fromChessPosition(new_pos);
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

int Jugada::getNewPos()
{
    return new_pos;
}