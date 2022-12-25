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

Pieza* Pieza::create(int pos, char nombre)
{
    switch (nombre){
        case 'B':
            return new Alfil(pos, true);
            break;
        case 'b':
            return new Alfil(pos, false);
            break;
        case 'N':
            return new Caballo(pos, true);
            break;
        case 'n':
            return new Caballo(pos, false);
            break;
        case 'R':
            return new Torre(pos, true);
            break;
        case 'r':
            return new Torre(pos, false);
            break;
        case 'P':
            return new Peon(pos, true);
            break;
        case 'p':
            return new Peon(pos, false);
            break;
        case 'K':
            return new Rey(pos, true);
            break;
        case 'k':
            return new Rey(pos, false);
            break;
        case 'Q':
            return new Dama(pos, true);
            break;
        case 'q':
            return new Dama(pos, false);
            break;
        default:
            return nullptr;
            break;
    }
}

Pieza::Pieza(int pos, bool blancas)
{
    this->pos = pos;
    this->blancas = blancas;
    this->timesMoved = 0;
};

char Pieza::getNameFEN()
{
    return nameFEN;
}

char Pieza::getNameNotation()
{
    return nameNotation;
}

bool Pieza::getColor()
{
    return blancas;
}

int Pieza::getPos()
{
    return pos;
}

int Pieza::getTimesMoved()
{
    return timesMoved;
}

void Pieza::move(int pos, bool moved)
{

    this->pos = pos;
    m_sprite.setPosition(pos % 8 * OBJECT_SIZE_F + OBJECT_SIZE_F/2, (7 - (pos / 8)) * OBJECT_SIZE_F + OBJECT_SIZE_F/2);
    if (moved) // really move
    {
        timesMoved++;
    }
    else // undo
    {
        timesMoved--;
    }
}

void Pieza::move(std::string pos)
{
    move(fromChessPosition(pos));
}

// void Pieza::loadTexture(std::string filePath)
// {
//     if (!texture.loadFromFile(filePath))
//         std::cout << "Error loading file\n";
//     texture.setSmooth(true);
// }

// void Pieza::setTexture()
// {
//     m_sprite.setTexture(texture);

//     int spriteSizeX = m_sprite.getTexture()->getSize().x;
//     int spriteSizeY = m_sprite.getTexture()->getSize().y;

//     m_sprite.setOrigin(sf::Vector2f(spriteSizeX / 2, spriteSizeY / 2));
//     m_sprite.setScale(sf::Vector2f(0.75f*OBJECT_SIZE/spriteSizeX, 0.75f*OBJECT_SIZE/spriteSizeY));
//     m_sprite.setPosition(pos % 8 * OBJECT_SIZE_F + OBJECT_SIZE_F/2, (7 - (pos / 8)) * OBJECT_SIZE_F + OBJECT_SIZE_F/2); // vista blancas
//     // m_sprite.setPosition((7-(pos%8)) * 64.0f + 32.0f, pos/8 * 64.0f + 32.0f); // vista negras
// }

void Pieza::setTexture()
{
    m_sprite = sf::Sprite();
    switch (nameNotation)
    {
        case 'K':
            m_sprite.setTexture(blancas ? PieceTextures::whiteKing : PieceTextures::blackKing);
            break;
        case 'Q':
            m_sprite.setTexture(blancas ? PieceTextures::whiteQueen : PieceTextures::blackQueen);
            break;
        case 'R':
            m_sprite.setTexture(blancas ? PieceTextures::whiteRook : PieceTextures::blackRook);
            break;
        case 'B':
            m_sprite.setTexture(blancas ? PieceTextures::whiteBishop : PieceTextures::blackBishop);
            break;
        case 'N':
            m_sprite.setTexture(blancas ? PieceTextures::whiteKnight : PieceTextures::blackKnight);
            break;
        case 'P':
            m_sprite.setTexture(blancas ? PieceTextures::whitePawn : PieceTextures::blackPawn);
            break;
        default:
            std::cerr << "Error piece type does not exist.\n";
            break;
    }

    int spriteSizeX = m_sprite.getTexture()->getSize().x;
    int spriteSizeY = m_sprite.getTexture()->getSize().y;

    m_sprite.setOrigin(sf::Vector2f(spriteSizeX/2 , spriteSizeY/2));
    m_sprite.setScale(sf::Vector2f(0.75f*OBJECT_SIZE/spriteSizeX, 0.75f*OBJECT_SIZE/spriteSizeY));
    m_sprite.setPosition(pos % 8 * OBJECT_SIZE_F + OBJECT_SIZE_F/2, (7 - (pos / 8)) * OBJECT_SIZE_F + OBJECT_SIZE_F/2); // vista blancas
    //m_sprite.setPosition((7-(pos%8)) * 64.0f + 32.0f, pos/8 * 64.0f + 32.0f); // vista negras
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

void Pieza::setPos(int pos){
    this->pos = pos;
}

// ALFIL

Alfil::Alfil(int pos, bool blancas) : Pieza(pos, blancas)
{
    if (blancas){
        this->nameFEN = 'B';
    }
    else {
        this->nameFEN = 'b';
    }
    this->nameNotation = 'B';
    setTexture();
    //std::string name = "bishop";
    //this->texture_file = ("img/" + std::string(blancas ? "w_" : "b_") + name + ".png");
    //loadTexture(this->texture_file);
    //setTexture();
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

Caballo::Caballo(int pos, bool blancas) : Pieza(pos, blancas)
{
    if (blancas){
        this->nameFEN = 'N';
    }
    else {
        this->nameFEN = 'n';
    }
    this->nameNotation = 'N';
    setTexture();
    // std::string name = "knight";
    // this->texture_file = ("img/" + std::string(blancas ? "w_" : "b_") + name + ".png");
    // loadTexture(this->texture_file);
    // setTexture();
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

Torre::Torre(int pos, bool blancas) : Pieza(pos, blancas)
{
    if (blancas){
        this->nameFEN = 'R';
    }
    else {
        this->nameFEN = 'r';
    }
    this->nameNotation = 'R';
    setTexture();
    // std::string name = "rook";
    // this->texture_file = ("img/" + std::string(blancas ? "w_" : "b_") + name + ".png");
    // loadTexture(this->texture_file);
    // setTexture();
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

Peon::Peon(int pos, bool blancas) : Pieza(pos, blancas)
{
    if (blancas){
        this->nameFEN = 'P';
    }
    else {
        this->nameFEN = 'p';
    }
    this->nameNotation = 'P';
    setTexture();
    // std::string name = "pawn";
    // this->texture_file = ("img/" + std::string(blancas ? "w_" : "b_") + name + ".png");
    // loadTexture(this->texture_file);
    // setTexture();
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

    if (timesMoved == 0)
    {
        possibleMovs.push_back(tmp + posRel); // extra move
    }

    if (tmp >= 0 || tmp <= 63)
    {
        possibleMovs.push_back(tmp);
        if (tmp / 8 == (tmp -1) / 8){
            possibleMovs.push_back(tmp - 1); // eat
        }
        if (tmp / 8 == (tmp +1) / 8){
            possibleMovs.push_back(tmp + 1); // eat
        }
    }

    return possibleMovs;
}

// REY

Rey::Rey(int pos, bool blancas) : Pieza(pos, blancas)
{
    if (blancas){
        this->nameFEN = 'K';
    }
    else {
        this->nameFEN = 'k';
    }
    this->nameNotation = 'K';
    setTexture();
    // std::string name = "king";
    // this->texture_file = ("img/" + std::string(blancas ? "w_" : "b_") + name + ".png");
    // loadTexture(this->texture_file);
    // setTexture();
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
    if (timesMoved == 0)
    {
        possibleMovs.push_back(pos + 2);
        possibleMovs.push_back(pos - 2);
    }

    return possibleMovs;
}

// DAMA

Dama::Dama(int pos, bool blancas) : Pieza(pos, blancas)
{
    if (blancas){
        this->nameFEN = 'Q';
    }
    else {
        this->nameFEN = 'q';
    }
    this->nameNotation = 'Q';
    setTexture();
    // std::string name = "queen";
    // this->texture_file = ("img/" + std::string(blancas ? "w_" : "b_") + name + ".png");
    // loadTexture(this->texture_file);
    // setTexture();
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