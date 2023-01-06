#include "promotionComponent.hpp"
#include "configuration.cpp"
#include "pieceTextures.hpp"

PromotionComponent::PromotionComponent(int pos, bool color)
{
    posX = (pos % 8);
    this->color = color;

    if (color)
    {
        sf::Color whiteColor = sf::Color(0xffd70070);

        whitePromotionChoices[0].setPosition(sf::Vector2f(posX * OBJECT_SIZE_F, 0 * OBJECT_SIZE_F));
        whitePromotionChoices[0].setSize(sf::Vector2f(OBJECT_SIZE_F, OBJECT_SIZE_F));
        whitePromotionChoices[0].setFillColor(whiteColor);
        whitePromotionChoices[0].setTexture(&PieceTextures::whiteQueen);
        whitePromotionChoices[1].setPosition(sf::Vector2f(posX * OBJECT_SIZE_F, 1 * OBJECT_SIZE_F));
        whitePromotionChoices[1].setSize(sf::Vector2f(OBJECT_SIZE_F, OBJECT_SIZE_F));
        whitePromotionChoices[1].setFillColor(whiteColor);
        whitePromotionChoices[1].setTexture(&PieceTextures::whiteKnight);
        whitePromotionChoices[2].setPosition(sf::Vector2f(posX * OBJECT_SIZE_F, 2 * OBJECT_SIZE_F));
        whitePromotionChoices[2].setSize(sf::Vector2f(OBJECT_SIZE_F, OBJECT_SIZE_F));
        whitePromotionChoices[2].setFillColor(whiteColor);
        whitePromotionChoices[2].setTexture(&PieceTextures::whiteRook);
        whitePromotionChoices[3].setPosition(sf::Vector2f(posX * OBJECT_SIZE_F, 3 * OBJECT_SIZE_F));
        whitePromotionChoices[3].setSize(sf::Vector2f(OBJECT_SIZE_F, OBJECT_SIZE_F));
        whitePromotionChoices[3].setFillColor(whiteColor);
        whitePromotionChoices[3].setTexture(&PieceTextures::whiteBishop);
    }
    else 
    {
        sf::Color blackColor = sf::Color(0xffd70070);

        blackPromotionChoices[0].setPosition(sf::Vector2f(posX * OBJECT_SIZE_F, 0 * OBJECT_SIZE_F));
        blackPromotionChoices[0].setSize(sf::Vector2f(OBJECT_SIZE_F, OBJECT_SIZE_F));
        blackPromotionChoices[0].setFillColor(blackColor);
        blackPromotionChoices[0].setTexture(&PieceTextures::blackQueen);
        blackPromotionChoices[1].setPosition(sf::Vector2f(posX * OBJECT_SIZE_F, 1 * OBJECT_SIZE_F));
        blackPromotionChoices[1].setSize(sf::Vector2f(OBJECT_SIZE_F, OBJECT_SIZE_F));
        blackPromotionChoices[1].setFillColor(blackColor);
        blackPromotionChoices[1].setTexture(&PieceTextures::blackKnight);
        blackPromotionChoices[2].setPosition(sf::Vector2f(posX * OBJECT_SIZE_F, 2 * OBJECT_SIZE_F));
        blackPromotionChoices[2].setSize(sf::Vector2f(OBJECT_SIZE_F, OBJECT_SIZE_F));
        blackPromotionChoices[2].setFillColor(blackColor);
        blackPromotionChoices[2].setTexture(&PieceTextures::blackRook);
        blackPromotionChoices[3].setPosition(sf::Vector2f(posX * OBJECT_SIZE_F, 3 * OBJECT_SIZE_F));
        blackPromotionChoices[3].setSize(sf::Vector2f(OBJECT_SIZE_F, OBJECT_SIZE_F));
        blackPromotionChoices[3].setFillColor(blackColor);
        blackPromotionChoices[3].setTexture(&PieceTextures::blackBishop);
    }
}

void PromotionComponent::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (color)
    {
        for (int i = 0; i < 4; i++)
        {
            target.draw(whitePromotionChoices[i]);
        }
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            target.draw(blackPromotionChoices[i]);
        }
    }
    
}
