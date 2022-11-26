#pragma once

class Pieza
{

protected:
    char nombre;
    bool blancas;
    int file;
    int col;

public:
    Pieza(int file, int col, bool blancas);
    // getters
    bool getColor();
    int getFile();
    int getCol();
    char getNombre();
    // setters
    // methods
    virtual bool calcularMovimiento() = 0; // calcula los movimientos posibles de la pieza (abstracto)
    void move(int file, int col);
};

// ALFIL

class Alfil : public Pieza
{
public:
    Alfil(int file, int column, bool blancas);
    virtual bool calcularMovimiento();
};

// Caballo

class Caballo : public Pieza
{
public:
    Caballo(int file, int column, bool blancas);
    virtual bool calcularMovimiento();
};

// PEON

class Peon : public Pieza
{
public:
    Peon(int file, int column, bool blancas);
    virtual bool calcularMovimiento();
};

// TORRE

class Torre : public Pieza
{
public:
    Torre(int file, int column, bool blancas);
    virtual bool calcularMovimiento();
};

// REY

class Rey : public Pieza
{
public:
    Rey(int file, int column, bool blancas);
    virtual bool calcularMovimiento();
};

// DAMA

class Dama : public Pieza
{
public:
    Dama(int file, int column, bool blancas);
    virtual bool calcularMovimiento();
};