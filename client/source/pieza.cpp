
#include "pieza.hpp"

// ABSTRACT

Pieza::Pieza(int file, int col, bool blancas)
{
    this->file=file;
    this->col=col;
    this->blancas=blancas;
};

char Pieza::getNombre(){ 
    return nombre;
}

bool Pieza::getColor(){
    return blancas;
}

int Pieza::getFile(){
    return file;
}

int Pieza::getCol(){
    return col;
}

void Pieza::move(int file, int col){
    this->file = file;
    this->col = col;
}

// ALFIL

Alfil::Alfil(int file, int col, bool blancas) : Pieza(file, col, blancas)
{
    this->nombre = 'A';
}

bool Alfil::calcularMovimiento(){
    return true;
}

// CABALLO

Caballo::Caballo(int file, int col, bool blancas) : Pieza(file, col, blancas)
{
    this->nombre = 'C';
}

bool Caballo::calcularMovimiento(){
    return true;
}

// TORRE

Torre::Torre(int file, int col, bool blancas) : Pieza(file, col, blancas)
{
    this->nombre = 'T';
}

bool Torre::calcularMovimiento(){
    return true;
}

// PEON

Peon::Peon(int file, int col, bool blancas) : Pieza(file, col, blancas)
{
    this->nombre = 'P';
}

bool Peon::calcularMovimiento(){
    return true;
}

// REY

Rey::Rey(int file, int col, bool blancas) : Pieza(file, col, blancas)
{
    this->nombre = 'R';
}

bool Rey::calcularMovimiento(){
    return true;
}

// DAMA

Dama::Dama(int file, int col, bool blancas) : Pieza(file, col, blancas)
{
    this->nombre = 'D';
}

bool Dama::calcularMovimiento(){
    return true;
}