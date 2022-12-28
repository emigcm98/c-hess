#include "move.hpp"
// #include "partida.hpp"

Move::Move(Piece *piece, std::string newPos) : Move(piece, fromChessPosition(newPos)){}

Move::Move(Piece *piece, int newPos)
{
    this->piece = piece;
    this->prevPos = piece->getPos();
    this->newPos = newPos;
    this->newPosStr = toChessPosition(newPos);
    if (!instanceof<Pawn>(piece))
    {
        this->pieceNameNotation = piece->getNameNotation();
    }

    this->shortCastling = false;
    this->longCastling = false;
    this->firstPieceMove = false;
    this->enPassant = false;
    this->check = false;
    this->checkmate = false;
    this->promoted = false;
    this->eatenPiece = nullptr;
}

Piece *Move::getPiece()
{
    return this->piece;
}

int Move::getPrevPos()
{
    return prevPos;
}

int Move::getNewPos()
{
    return newPos;
}

bool Move::isCheck()
{
    return check;
}

bool Move::isCheckmate()
{
    return checkmate;
}

void Move::generateString()
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
        if (pieceNameNotation != '\0')
        {
            str += pieceNameNotation;
        }
        if (eatenPiece != nullptr){
            if (pieceNameNotation == '\0')
            {
                str += ((prevPos%8) + 'a');
            }
            str += 'x';
        }
        str += newPosStr;
        if (promoted){
            str += '=';
            str += piece->getNameNotation();
        }
        if (check){
            str += '+';
            if (checkmate){
                str += '+';
            }
        }
        //return std::string(1, piece->getNombre()) + newPosStr; //+ (isJaque ? "+" : "") + (isJaqueMate ? "+" : "");
    }
}

std::string Move::to_string() {
    return str;
}

void Move::shortCastle() {
    shortCastling = true;
}

void Move::longCastle() {
    longCastling = true;
}

bool Move::isShortCastling() {
    return shortCastling;
}

bool Move::isLongCastling() {
    return longCastling;
}

bool Move::isFirstPieceMove() {
    return firstPieceMove;
}

void Move::setFirstPieceMoved(bool moved) {
    firstPieceMove = moved;
    //piece->setMoved(moved);
}

void Move::setCheck(bool check) {
    this->check = check;
}

void Move::setCheckmate(bool checkmate) {
    this->checkmate = checkmate;
}

void Move::setPromoted(bool promoted){
    this->promoted = promoted;
}

void Move::eat(Piece* p) {
    this->eatenPiece = p;
}

Piece* Move::getEatenPiece() {
    return eatenPiece;
}

void Move::setPiece(Piece *p) {
    this->piece = p;
}