#include <SFML/Graphics.hpp>
#include <vector>
#include "jugada.hpp"
#include "user.hpp"

class GameInfo : public sf::Drawable
{
private:
    //User* blancas;
    //User* negras;
    std::vector<Jugada*> *jugadas;
    //sf::Font* font;
    sf::RectangleShape container;
    sf::Text nombreBlancas;
    sf::Text nombreNegras;
    sf::Text eloBlancas;
    sf::Text eloNegras;
    sf::Text lastPlay;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    GameInfo(User* blancas, User* negras, std::vector<Jugada*> *jugadas, sf::Font* font);
    void updateJugada();
    void updateElos(int whiteElo, int blackElo);
};