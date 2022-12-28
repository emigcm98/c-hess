#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>

#include "pieceTextures.hpp"

#pragma once

template <typename Base, typename T>
inline bool instanceof (const T *ptr)
{
    return dynamic_cast<const Base *>(ptr) != nullptr;
}

int fromChessPosition(std::string cp);
std::string toChessPosition(int pos);

class Piece : public sf::Drawable
{

protected:
    char nameFEN;
    char nameNotation;
    bool color;
    int pos;
    //bool moved;
    int timesMoved; 

    //std::string texture_file;
    sf::Sprite m_sprite;
    //sf::Texture texture;
    //void loadTexture(std::string filePath);
    void setTexture();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {   target.draw(m_sprite);  }

public:
    static Piece* create(int pos, char nombre);
    Piece(int pos, bool color); // pos from 0 to 63
    Piece(std::string pos, bool color) : Piece(fromChessPosition(pos), color){}; // pos from 0 to 63
    // getters
    int getPos();
    bool getColor();
    char getNameFEN();
    char getNameNotation();
    int getTimesMoved();
    // setters
    //void setMoved(bool moved);
    void setPos(int pos);
    // methods
    virtual std::vector<int> calcularMovimiento() = 0; // calcula los movimientos posibles de la Piece (abstracto)
    void move(int pos, bool moved = true);
    void move(std::string pos);
    void rotate(bool orientation);
};

// Bishop

class Bishop : public Piece
{
public:
    Bishop(int pos, bool color);
    Bishop(std::string pos, bool color) : Bishop(fromChessPosition(pos), color){};
    virtual std::vector<int> calcularMovimiento();
};

// Knight

class Knight : public Piece
{
public:
    Knight(int pos, bool color);
    Knight(std::string pos, bool color) : Knight(fromChessPosition(pos), color){};
    virtual std::vector<int> calcularMovimiento();
};

// Pawn

class Pawn : public Piece
{
public:
    Pawn(int pos, bool color);
    Pawn(std::string pos, bool color) : Pawn(fromChessPosition(pos), color){};
    virtual std::vector<int> calcularMovimiento();
};

// Rook

class Rook : public Piece
{
public:
    Rook(int pos, bool color);
    Rook(std::string pos, bool color) : Rook(fromChessPosition(pos), color){};
    virtual std::vector<int> calcularMovimiento();
};

// King

class King : public Piece
{
public:
    King(int pos, bool color);
    King(std::string pos, bool color) : King(fromChessPosition(pos), color){};
    virtual std::vector<int> calcularMovimiento();
};

// Queen

class Queen : public Piece
{
public:
    Queen(int pos, bool color);
    Queen(std::string pos, bool color) : Queen(fromChessPosition(pos), color){};
    virtual std::vector<int> calcularMovimiento();
};