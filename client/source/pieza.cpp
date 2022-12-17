#include <math.h>

#include "pieza.hpp"
#include "configuration.cpp"

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
    char row = pos / 8 + 49;
    char col = pos % 8 + 97;

    return std::string(1, col) + row;
}

Pieza::Pieza(std::string pos, bool blancas)
{
    this->pos = fromChessPosition(pos);
    this->blancas = blancas;
    this->moved = false;
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

bool Pieza::getMoved()
{
    return moved;
}

void Pieza::move(int pos)
{

    this->pos = pos;
    m_sprite.setPosition(pos % 8 * OBJECT_SIZE_F + OBJECT_SIZE_F/2, (7 - (pos / 8)) * OBJECT_SIZE_F + OBJECT_SIZE_F/2);
    moved = true;
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

void Pieza::setTexture()
{
    m_sprite.setTexture(texture);

    int spriteSizeX = m_sprite.getTexture()->getSize().x;
    int spriteSizeY = m_sprite.getTexture()->getSize().y;

    m_sprite.setOrigin(sf::Vector2f(spriteSizeX / 2, spriteSizeY / 2));
    m_sprite.setScale(sf::Vector2f(0.75f*OBJECT_SIZE/spriteSizeX, 0.75f*OBJECT_SIZE/spriteSizeY));
    m_sprite.setPosition(pos % 8 * OBJECT_SIZE_F + OBJECT_SIZE_F/2, (7 - (pos / 8)) * OBJECT_SIZE_F + OBJECT_SIZE_F/2); // vista blancas
    // m_sprite.setPosition((7-(pos%8)) * 64.0f + 32.0f, pos/8 * 64.0f + 32.0f); // vista negras
}

void Pieza::rotate(bool orientation){
    if (orientation)
    {
        m_sprite.setPosition(pos % 8 * OBJECT_SIZE_F + OBJECT_SIZE_F/2, (7 - (pos / 8)) * OBJECT_SIZE_F + OBJECT_SIZE_F/2); // vista blancas
    }
    else {
        m_sprite.setPosition((7-(pos%8)) * OBJECT_SIZE_F + OBJECT_SIZE_F/2, pos/8 * OBJECT_SIZE_F + OBJECT_SIZE_F/2);
    }
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

    int posRel[4] = {9, -9, 7, -7};
    for (int i = 0; i < 4; i++)
    {

        int tmp = pos + posRel[i];

        for (int j = tmp; (j <= 63 && j >= 0); j += posRel[i])
        {
            if ((abs(j / 8 - pos / 8) - abs(j % 8 - pos % 8)) == 0)
            {
                possibleMovs.push_back(j);
            }
        }
    }
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
    for (int i = 0; i < 4; i++)
    {
        int tmp = pos + posRel[i];
        int tmp2 = pos - posRel[i];

        if (tmp <= 63 && ((abs(pos % 8 - tmp % 8) + abs(pos / 8 - tmp / 8)) == 3))
        {
            possibleMovs.push_back(tmp);
        }
        if (tmp2 >= 0 && ((abs(pos % 8 - tmp2 % 8) + abs(pos / 8 - tmp2 / 8)) == 3))
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

    int posRel[4] = {1, 8, -8, -1};
    for (int i = 0; i < 4; i++)
    {

        int tmp = pos + posRel[i];

        int times = 0;
        for (int j = tmp; (j <= 63 && j >= 0 && times < 7); j += posRel[i])
        {

            if ((abs(j / 8 - pos / 8) * abs(j % 8 - pos % 8)) == 0)
            {
                possibleMovs.push_back(j);
            }
            times++;
        }
    }
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

    int posRel = 8;

    if (!blancas)
    {
        posRel = -posRel;
    }

    int tmp = pos + posRel;

    if (!moved)
    {
        possibleMovs.push_back(tmp + posRel); // extra move
    }

    if (tmp >= 0 || tmp <= 63)
    {
        possibleMovs.push_back(tmp - 1); // eat
        possibleMovs.push_back(tmp);
        possibleMovs.push_back(tmp + 1); // eat
    }

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

    int posRel[4] = {1, 7, 8, 9};
    for (int i = 0; i < 4; i++)
    {

        int tmp = pos + posRel[i];
        int tmp2 = pos - posRel[i];

        if (tmp <= 63 && ((abs(tmp / 8 - pos / 8) + abs(tmp % 8 - pos % 8)) == 1 || (abs(tmp / 8 - pos / 8) + abs(tmp % 8 - pos % 8)) == 2))
        {
            possibleMovs.push_back(tmp);
        }
        if (tmp2 >= 0 && ((abs(tmp2 / 8 - pos / 8) + abs(tmp2 % 8 - pos % 8)) == 1 || (abs(tmp2 / 8 - pos / 8) + abs(tmp2 % 8 - pos % 8)) == 2))
        {
            possibleMovs.push_back(tmp2);
        }
    }

    // enroque
    if (!moved)
    {
        possibleMovs.push_back(pos + 2);
        possibleMovs.push_back(pos - 2);
    }

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

    int posRel[4] = {1, 7, 8, 9};
    for (int i = 0; i < 4; i++)
    {

        int tmp = pos + posRel[i];
        int tmp2 = pos - posRel[i];

        int times = 0;
        for (int j = tmp; (j <= 63 && j >= 0 && times < 7); j += posRel[i])
        {

            if ((abs(j / 8 - pos / 8) * abs(j % 8 - pos % 8)) == 0 || (abs(j / 8 - pos / 8) - abs(j % 8 - pos % 8)) == 0)
            {
                possibleMovs.push_back(j);
            }
            times++;
        }
        times = 0;
        for (int j = tmp2; (j <= 63 && j >= 0 && times < 7); j -= posRel[i])
        {
            if ((abs(j / 8 - pos / 8) * abs(j % 8 - pos % 8)) == 0 || (abs(j / 8 - pos / 8) - abs(j % 8 - pos % 8)) == 0)
            {
                possibleMovs.push_back(j);
            }
            times++;
        }
    }
    return possibleMovs;
}