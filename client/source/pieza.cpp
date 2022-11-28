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

std::vector<int> Alfil::calcularMovimiento()
{
    std::vector<int> possibleMovs;
    return possibleMovs;
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

std::vector<int> Caballo::calcularMovimiento()
{
    std::vector<int> possibleMovs;

    int posRel[4] = {6, 10, 15, 17};
    for (int i = 0; i < 4; i++){
        int tmp = pos+posRel[i];
        int tmp2 = pos-posRel[i];
        if (tmp <= 63)
        {
            possibleMovs.push_back(tmp);
        }
        if (tmp2 >= 0)
        {
            possibleMovs.push_back(tmp2);
        }
    }
    return possibleMovs;
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

std::vector<int> Torre::calcularMovimiento()
{
    std::vector<int> possibleMovs;
    return possibleMovs;
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

std::vector<int> Peon::calcularMovimiento()
{
    std::vector<int> possibleMovs;
    return possibleMovs;
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

std::vector<int> Rey::calcularMovimiento()
{
    std::vector<int> possibleMovs;
    return possibleMovs;
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

std::vector<int> Dama::calcularMovimiento()
{
    std::vector<int> possibleMovs;
    return possibleMovs;
}