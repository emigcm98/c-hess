#include "jugada.hpp"
// #include "partida.hpp"

Jugada::Jugada(Pieza *pieza, std::string newPos)
{
    // Jugada(pieza, fromChessPosition(newPos));
    this->pieza = pieza;
    this->prevPos = pieza->getPos();
    this->newPos = fromChessPosition(newPos);
    this->newPosStr = newPos;
    this->shortCastling = false;
    this->longCastling = false;
    this->firstPieceMove = false;
    this->enPassant = false;
    check_movement();
}

Jugada::Jugada(Pieza *pieza, int newPos)
{
    this->pieza = pieza;
    this->prevPos = pieza->getPos();
    this->newPos = newPos;
    this->newPosStr = newPos;
    check_movement();
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

    // this->pieza->move(this->newPos);

    return is_possible;
}

Pieza *Jugada::getPieza()
{
    return this->pieza;
}

int Jugada::getNewPos()
{
    return newPos;
}

bool Jugada::isJaque()
{
    return false;
}

bool Jugada::isJaqueMate()
{
    return false;
}

std::string Jugada::to_string()
{
    if (shortCastling)
    {
        return "0-0";
    }
    else if (longCastling)
    {
        return "0-0-0";
    }
    else
    {
        return std::string(1, pieza->getNombre()) + newPosStr; //+ (isJaque ? "+" : "") + (isJaqueMate ? "+" : "");
    }
}

void Jugada::shortCastle() {
    shortCastling = true;
}

void Jugada::longCastle() {
    longCastling = true;
}

bool Jugada::isShortCastling() {
    return shortCastling;
}

bool Jugada::isLongCastling() {
    return longCastling;
}

bool Jugada::isFirstPieceMove() {
    return firstPieceMove;
}

void Jugada::firstPieceMoved() {
    firstPieceMove = true;
}