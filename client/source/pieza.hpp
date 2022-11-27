#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#pragma once

int fromChessPosition(std::string cp);
std::string toChessPosition(int pos);

class Pieza : public sf::Drawable
{

protected:
    char nombre;
    bool blancas;
    int pos; 

    std::string texture_file;
    sf::Sprite m_sprite;
    sf::Texture texture;
    void loadTexture(std::string filePath);
    void setTexture();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {   target.draw(m_sprite);  }

public:
    Pieza(std::string pos, bool blancas); // pos from 0 to 63
    // getters
    int getPos();
    bool getColor();
    char getNombre();
    // setters
    // methods
    virtual bool calcularMovimiento() = 0; // calcula los movimientos posibles de la pieza (abstracto)
    void move(int pos);
    void move(std::string pos);
};

// ALFIL

class Alfil : public Pieza
{
public:
    Alfil(std::string pos, bool blancas);
    virtual bool calcularMovimiento();
};

// Caballo

class Caballo : public Pieza
{
public:
    Caballo(std::string pos, bool blancas);
    virtual bool calcularMovimiento();
};

// PEON

class Peon : public Pieza
{
public:
    Peon(std::string pos, bool blancas);
    virtual bool calcularMovimiento();
};

// TORRE

class Torre : public Pieza
{
public:
    Torre(std::string pos, bool blancas);
    virtual bool calcularMovimiento();
};

// REY

class Rey : public Pieza
{
public:
    Rey(std::string pos, bool blancas);
    virtual bool calcularMovimiento();
};

// DAMA

class Dama : public Pieza
{
public:
    Dama(std::string pos, bool blancas);
    virtual bool calcularMovimiento();
};