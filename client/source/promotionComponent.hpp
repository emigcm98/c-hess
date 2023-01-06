#include <SFML/Graphics.hpp>
#include <array>

class PromotionComponent : public sf::Drawable
{
private:
    std::array<sf::RectangleShape, 4> whitePromotionChoices;
    std::array<sf::RectangleShape, 4> blackPromotionChoices;
    int posX;
    bool color;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

public:
    PromotionComponent(int pos, bool color);
};