#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>

#pragma once

int fromChessPosition(std::string cp);
std::string toChessPosition(int pos);

class Pieza : public sf::Drawable
{

protected:
    char nombre;
    bool blancas;
    int pos;
    bool moved; 

    std::string texture_file;
    sf::Sprite m_sprite;
    sf::Texture texture;
    void loadTexture(std::string filePath);
    void setTexture();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {   target.draw(m_sprite);  }

public:
    Pieza(int pos, char nombre);
    Pieza(int pos, bool blancas); // pos from 0 to 63
    Pieza(std::string pos, bool blancas) : Pieza(fromChessPosition(pos), blancas){}; // pos from 0 to 63
    // getters
    int getPos();
    bool getColor();
    char getNombre();
    bool getMoved();
    // setters
    // methods
    virtual std::vector<int> calcularMovimiento() = 0; // calcula los movimientos posibles de la pieza (abstracto)
    void move(int pos);
    void move(std::string pos);
    void rotate(bool orientation);
};

// ALFIL

class Alfil : public Pieza
{
public:
    Alfil(int pos, bool blancas);
    Alfil(std::string pos, bool blancas) : Alfil(fromChessPosition(pos), blancas){};
    virtual std::vector<int> calcularMovimiento();
};

// Caballo

class Caballo : public Pieza
{
public:
    Caballo(int pos, bool blancas);
    Caballo(std::string pos, bool blancas) : Caballo(fromChessPosition(pos), blancas){};
    virtual std::vector<int> calcularMovimiento();
};

// PEON

class Peon : public Pieza
{
public:
    Peon(int pos, bool blancas);
    Peon(std::string pos, bool blancas) : Peon(fromChessPosition(pos), blancas){};
    virtual std::vector<int> calcularMovimiento();
};

// TORRE

class Torre : public Pieza
{
public:
    Torre(int pos, bool blancas);
    Torre(std::string pos, bool blancas) : Torre(fromChessPosition(pos), blancas){};
    virtual std::vector<int> calcularMovimiento();
};

// REY

class Rey : public Pieza
{
public:
    Rey(int pos, bool blancas);
    Rey(std::string pos, bool blancas) : Rey(fromChessPosition(pos), blancas){};
    virtual std::vector<int> calcularMovimiento();
};

// DAMA

class Dama : public Pieza
{
public:
    Dama(int pos, bool blancas);
    Dama(std::string pos, bool blancas) : Dama(fromChessPosition(pos), blancas){};
    virtual std::vector<int> calcularMovimiento();
};