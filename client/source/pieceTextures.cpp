/*
    This code file contains static member initialisations
    of pieceTextures.h
*/

#include "pieceTextures.hpp"

sf::Texture PieceTextures::loadTexture(std::string str){
    sf::Texture tmp;
    if (!tmp.loadFromFile(str))
        std::cout << "Error loading file\n";
    tmp.setSmooth(true);
    return tmp;
}

sf::Texture PieceTextures::blackKing = PieceTextures::loadTexture("img/b_king.png");
sf::Texture PieceTextures::blackQueen = PieceTextures::loadTexture("img/b_queen.png");
sf::Texture PieceTextures::blackRook = PieceTextures::loadTexture("img/b_rook.png");
sf::Texture PieceTextures::blackKnight = PieceTextures::loadTexture("img/b_knight.png");
sf::Texture PieceTextures::blackBishop = PieceTextures::loadTexture("img/b_bishop.png");
sf::Texture PieceTextures::blackPawn = PieceTextures::loadTexture("img/b_pawn.png");

sf::Texture PieceTextures::whiteKing = PieceTextures::loadTexture("img/w_king.png");
sf::Texture PieceTextures::whiteQueen = PieceTextures::loadTexture("img/w_queen.png");
sf::Texture PieceTextures::whiteRook = PieceTextures::loadTexture("img/w_rook.png");
sf::Texture PieceTextures::whiteKnight = PieceTextures::loadTexture("img/w_knight.png");
sf::Texture PieceTextures::whiteBishop = PieceTextures::loadTexture("img/w_bishop.png");
sf::Texture PieceTextures::whitePawn = PieceTextures::loadTexture("img/w_pawn.png");