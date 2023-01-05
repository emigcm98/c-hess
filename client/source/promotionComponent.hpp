#include <SFML/Graphics.hpp>
#include <array>

class PromotionComponent : public sf::Drawable
{
private:
    std::array<sf::RectangleShape, 4> whitePromotionChoices; // static
    std::array<sf::RectangleShape, 4> blackPromotionChoices; // static
    int posX;
    int posY;
    bool color;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

public:
    PromotionComponent(int pos, bool color);
};