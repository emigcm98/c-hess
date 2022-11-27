#include "pieza.hpp"

// ABSTRACT

int fromChessPosition(std::string pos)
{
    // a1
    if (pos.size() != 2)
        return -1;
    int row = pos.at(1) - 49;
    int col = pos.at(0) - 97;

    return (row)*8 + col;
}

std::string toChessPosition(int pos)
{
    if (pos < 0 || pos > 63)
        return nullptr;
    char row = pos/8 + 49; 
    char col = pos%8 + 97;
    
    return std::string(1, col) + row;

}

Pieza::Pieza(std::string pos, bool blancas)
{
    this->pos = fromChessPosition(pos);
    this->blancas = blancas; 
    m_sprite = sf::Sprite();
};

char Pieza::getNombre()
{
    return nombre;
}

bool Pieza::getColor()
{
    return blancas;
}

int Pieza::getPos()
{
    return pos;
}

void Pieza::move(int pos)
{
    this->pos = pos;
    m_sprite.setPosition(pos%8 * 96.0f + 48.0f, (7-(pos/8)) * 96.0f + 48.0f);
}

void Pieza::move(std::string pos)
{
    move(fromChessPosition(pos));
}

void Pieza::loadTexture(std::string filePath)
{
    if (!texture.loadFromFile(filePath))
        std::cout << "Error loading file\n";
    texture.setSmooth(true);
}

void Pieza::setTexture(){
    m_sprite.setTexture(texture);
    m_sprite.setOrigin(sf::Vector2f(m_sprite.getTexture()->getSize().x/2 , m_sprite.getTexture()->getSize().y/2));
    m_sprite.setScale(sf::Vector2f(0.625f,0.625f));
    m_sprite.setPosition(pos%8 * 96.0f + 48.0f, (7-(pos/8)) * 96.0f + 48.0f); // vista blancas
    //m_sprite.setPosition((7-(pos%8)) * 64.0f + 32.0f, pos/8 * 64.0f + 32.0f); // vista negras
}

// ALFIL

Alfil::Alfil(std::string pos, bool blancas) : Pieza(pos, blancas)
{
    this->nombre = 'A';
    std::string name = "bishop";
    this->texture_file = ("img/" + std::string(blancas ? "w_" : "b_") + name + ".png");
    loadTexture(this->texture_file);
    setTexture();
}

bool Alfil::calcularMovimiento()
{
    return true;
}

// CABALLO

Caballo::Caballo(std::string pos, bool blancas) : Pieza(pos, blancas)
{
    this->nombre = 'C';
    std::string name = "knight";
    this->texture_file = ("img/" + std::string(blancas ? "w_" : "b_") + name + ".png");
    loadTexture(this->texture_file);
    setTexture();
}

bool Caballo::calcularMovimiento()
{
    return true;
}

// TORRE

Torre::Torre(std::string pos, bool blancas) : Pieza(pos, blancas)
{
    this->nombre = 'T';
    std::string name = "rook";
    this->texture_file = ("img/" + std::string(blancas ? "w_" : "b_") + name + ".png");
    loadTexture(this->texture_file);
    setTexture();
}

bool Torre::calcularMovimiento()
{
    return true;
}

// PEON

Peon::Peon(std::string pos, bool blancas) : Pieza(pos, blancas)
{
    this->nombre = 'P';
    std::string name = "pawn";
    this->texture_file = ("img/" + std::string(blancas ? "w_" : "b_") + name + ".png");
    loadTexture(this->texture_file);
    setTexture();
}

bool Peon::calcularMovimiento()
{
    return true;
}

// REY

Rey::Rey(std::string pos, bool blancas) : Pieza(pos, blancas)
{
    this->nombre = 'R';
    std::string name = "king";
    this->texture_file = ("img/" + std::string(blancas ? "w_" : "b_") + name + ".png");
    loadTexture(this->texture_file);
    setTexture();
}

bool Rey::calcularMovimiento()
{
    return true;
}

// DAMA

Dama::Dama(std::string pos, bool blancas) : Pieza(pos, blancas)
{
    this->nombre = 'D';
    std::string name = "queen";
    this->texture_file = ("img/" + std::string(blancas ? "w_" : "b_") + name + ".png");
    loadTexture(this->texture_file);
    setTexture();
}

bool Dama::calcularMovimiento()
{
    return true;
}