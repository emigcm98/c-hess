#include <math.h>

#include "piece.hpp"
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

Piece* Piece::create(int pos, char nombre)
{
    switch (nombre){
        case 'B':
            return new Bishop(pos, true);
            break;
        case 'b':
            return new Bishop(pos, false);
            break;
        case 'N':
            return new Knight(pos, true);
            break;
        case 'n':
            return new Knight(pos, false);
            break;
        case 'R':
            return new Rook(pos, true);
            break;
        case 'r':
            return new Rook(pos, false);
            break;
        case 'P':
            return new Pawn(pos, true);
            break;
        case 'p':
            return new Pawn(pos, false);
            break;
        case 'K':
            return new King(pos, true);
            break;
        case 'k':
            return new King(pos, false);
            break;
        case 'Q':
            return new Queen(pos, true);
            break;
        case 'q':
            return new Queen(pos, false);
            break;
        default:
            return nullptr;
            break;
    }
}

Piece::Piece(int pos, bool color)
{
    this->pos = pos;
    this->color = color;
    this->timesMoved = 0;
}

Piece::~Piece()
{
}

char Piece::getNameFEN()
{
    return nameFEN;
}

char Piece::getNameNotation()
{
    return nameNotation;
}

bool Piece::getColor()
{
    return color;
}

int Piece::getPos()
{
    return pos;
}

int Piece::getTimesMoved()
{
    return timesMoved;
}

void Piece::move(int pos, bool moved)
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

void Piece::move(std::string pos)
{
    move(fromChessPosition(pos));
}

void Piece::setTexture()
{
    m_sprite = sf::Sprite();
    switch (nameNotation)
    {
        case 'K':
            m_sprite.setTexture(color ? PieceTextures::whiteKing : PieceTextures::blackKing);
            break;
        case 'Q':
            m_sprite.setTexture(color ? PieceTextures::whiteQueen : PieceTextures::blackQueen);
            break;
        case 'R':
            m_sprite.setTexture(color ? PieceTextures::whiteRook : PieceTextures::blackRook);
            break;
        case 'B':
            m_sprite.setTexture(color ? PieceTextures::whiteBishop : PieceTextures::blackBishop);
            break;
        case 'N':
            m_sprite.setTexture(color ? PieceTextures::whiteKnight : PieceTextures::blackKnight);
            break;
        case 'P':
            m_sprite.setTexture(color ? PieceTextures::whitePawn : PieceTextures::blackPawn);
            break;
        default:
            std::cerr << "Error piece type does not exist.\n";
            break;
    }

    int spriteSizeX = m_sprite.getTexture()->getSize().x;
    int spriteSizeY = m_sprite.getTexture()->getSize().y;

    m_sprite.setOrigin(sf::Vector2f(spriteSizeX/2 , spriteSizeY/2));
    m_sprite.setScale(sf::Vector2f(0.75f*OBJECT_SIZE/spriteSizeX, 0.75f*OBJECT_SIZE/spriteSizeY));
    m_sprite.setPosition(pos % 8 * OBJECT_SIZE_F + OBJECT_SIZE_F/2, (7 - (pos / 8)) * OBJECT_SIZE_F + OBJECT_SIZE_F/2); // vista color
    //m_sprite.setPosition((7-(pos%8)) * 64.0f + 32.0f, pos/8 * 64.0f + 32.0f); // vista negras
}

void Piece::rotate(bool orientation){
    if (orientation)
    {
        m_sprite.setPosition(pos % 8 * OBJECT_SIZE_F + OBJECT_SIZE_F/2, (7 - (pos / 8)) * OBJECT_SIZE_F + OBJECT_SIZE_F/2); // vista color
    }
    else {
        m_sprite.setPosition((7-(pos%8)) * OBJECT_SIZE_F + OBJECT_SIZE_F/2, pos/8 * OBJECT_SIZE_F + OBJECT_SIZE_F/2);
    }
}

void Piece::setPos(int pos){
    this->pos = pos;
}

// Bishop

Bishop::Bishop(int pos, bool color) : Piece(pos, color)
{
    if (color){
        this->nameFEN = 'B';
    }
    else {
        this->nameFEN = 'b';
    }
    this->nameNotation = 'B';
    setTexture();
    //std::string name = "bishop";
    //this->texture_file = ("img/" + std::string(color ? "w_" : "b_") + name + ".png");
    //loadTexture(this->texture_file);
    //setTexture();
}

Bishop::~Bishop()
{
    //std::cout << "Deleting " << (color ? "white" : "black") << " bishop" << std::endl;
}


std::vector<int> Bishop::calcularMovimiento()
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

// Knight

Knight::Knight(int pos, bool color) : Piece(pos, color)
{
    if (color){
        this->nameFEN = 'N';
    }
    else {
        this->nameFEN = 'n';
    }
    this->nameNotation = 'N';
    setTexture();
}

Knight::~Knight()
{
    //std::cout << "Deleting " << (color ? "white" : "black") << " knight" << std::endl;
}

std::vector<int> Knight::calcularMovimiento()
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

// Rook

Rook::Rook(int pos, bool color) : Piece(pos, color)
{
    if (color){
        this->nameFEN = 'R';
    }
    else {
        this->nameFEN = 'r';
    }
    this->nameNotation = 'R';
    setTexture();
}

Rook::~Rook()
{
    //std::cout << "Deleting " << (color ? "white" : "black") << " rook" << std::endl;
}

std::vector<int> Rook::calcularMovimiento()
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

// Pawn

Pawn::Pawn(int pos, bool color) : Piece(pos, color)
{
    if (color){
        this->nameFEN = 'P';
    }
    else {
        this->nameFEN = 'p';
    }
    this->nameNotation = 'P';
    setTexture();
}

Pawn::~Pawn()
{
    //std::cout << "Deleting " << (color ? "white" : "black") << " pawn" << std::endl;
}

std::vector<int> Pawn::calcularMovimiento()
{
    std::vector<int> possibleMovs;

    int posRel = 8;

    if (!color)
    {
        posRel = -posRel;
    }

    int tmp = pos + posRel;

    int originalRow;
    if (color){
        originalRow = 1;
    }
    else {
        originalRow = 6;
    }

    if (timesMoved == 0 && pos/8 == originalRow)
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

// King

King::King(int pos, bool color) : Piece(pos, color)
{
    if (color){
        this->nameFEN = 'K';
    }
    else {
        this->nameFEN = 'k';
    }
    this->nameNotation = 'K';
    setTexture();
}

King::~King()
{
    //std::cout << "Deleting " << (color ? "white" : "black") << " king" << std::endl;
}

std::vector<int> King::calcularMovimiento()
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

    int originalPosition;
    if (color)
    {
        originalPosition = fromChessPosition("e1");
    }
    else {
        originalPosition = fromChessPosition("e8");
    }

    // enroque
    if (timesMoved == 0 && originalPosition == pos)
    {
        possibleMovs.push_back(pos + 2);
        possibleMovs.push_back(pos - 2);
    }

    return possibleMovs;
}

// Queen

Queen::Queen(int pos, bool color) : Piece(pos, color)
{
    if (color){
        this->nameFEN = 'Q';
    }
    else {
        this->nameFEN = 'q';
    }
    this->nameNotation = 'Q';
    setTexture();
    // std::string name = "queen";
    // this->texture_file = ("img/" + std::string(color ? "w_" : "b_") + name + ".png");
    // loadTexture(this->texture_file);
    // setTexture();
}

Queen::~Queen()
{
    //std::cout << "Deleting " << (color ? "white" : "black") << " queen" << std::endl;
}

std::vector<int> Queen::calcularMovimiento()
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
            if ((pos % 8 == 7 && i == 3) || (pos % 8 == 0 && i == 1)){
                //   
            }
            else if ((abs(j / 8 - pos / 8) * abs(j % 8 - pos % 8)) == 0 || (abs(j / 8 - pos / 8) - abs(j % 8 - pos % 8)) == 0)
            {
                possibleMovs.push_back(j);
            }
            times++;
        }
        times = 0;
        for (int j = tmp2; (j <= 63 && j >= 0 && times < 7); j -= posRel[i])
        {
            if ((pos % 8 == 7 && i == 1) || (pos % 8 == 0 && i == 3)){
                //   
            }
            else if ((abs(j / 8 - pos / 8) * abs(j % 8 - pos % 8)) == 0 || (abs(j / 8 - pos / 8) - abs(j % 8 - pos % 8)) == 0)
            {
                possibleMovs.push_back(j);
            }
            times++;
        }
    }
    return possibleMovs;
}