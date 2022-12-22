#include "jugada.hpp"
// #include "partida.hpp"

Jugada::Jugada(Pieza *pieza, std::string newPos) : Jugada(pieza, fromChessPosition(newPos)){}

Jugada::Jugada(Pieza *pieza, int newPos)
{
    this->pieza = pieza;
    this->prevPos = pieza->getPos();
    this->newPos = newPos;
    this->newPosStr = toChessPosition(newPos);
    this->shortCastling = false;
    this->longCastling = false;
    this->firstPieceMove = false;
    this->enPassant = false;
    this->check = false;
    this->checkmate = false;
    this->eatenPiece = nullptr;
}

// bool Jugada::check_movement()
// {
//     bool is_possible = true;

//     // if (is_possible == false){
//     //     delete this;
//     // }
//     // else {
//     //     p->aplicarJugada(this);
//     // }

//     // this->pieza->move(this->newPos);

//     return is_possible;
// }

Pieza *Jugada::getPieza()
{
    return this->pieza;
}

int Jugada::getPrevPos()
{
    return prevPos;
}

int Jugada::getNewPos()
{
    return newPos;
}

bool Jugada::isJaque()
{
    return check;
}

bool Jugada::isJaqueMate()
{
    return checkmate;
}

void Jugada::generateString()
{
    if (shortCastling)
    {
        str = "0-0";
    }
    else if (longCastling)
    {
        str = "0-0-0";
    }
    else
    {
        if (!instanceof<Peon>(pieza))
        {
            str += pieza->getNameNotation();
        }
        if (eatenPiece != nullptr){
            str += 'x';
        }
        str += newPosStr;
        if (isJaque()){
            str += '+';
            if (isJaqueMate()){
                str += '+';
            }
        }
        //return std::string(1, pieza->getNombre()) + newPosStr; //+ (isJaque ? "+" : "") + (isJaqueMate ? "+" : "");
    }
}

std::string Jugada::to_string() {
    return str;
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

void Jugada::setFirstPieceMoved(bool moved) {
    firstPieceMove = moved;
    //pieza->setMoved(moved);
}

void Jugada::setJaque(bool check) {
    this->check = check;
}

void Jugada::setJaqueMate(bool checkmate) {
    this->checkmate = checkmate;
}

void Jugada::eat(Pieza* p) {
    this->eatenPiece = p;
}

Pieza* Jugada::getEatenPiece() {
    return eatenPiece;
}