#include <SFML/Graphics.hpp>
#include <vector>
#include "move.hpp"
#include "user.hpp"

class GameInfo : public sf::Drawable
{
private:
    //User* blancas;
    //User* negras;
    std::vector<Move*> *moves;
    //sf::Font* font;
    sf::RectangleShape container;
    sf::Text whiteName;
    sf::Text blackName;
    sf::Text whiteElo;
    sf::Text blackElo;
    sf::Text lastMove;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    GameInfo(User* whitePlayer, User* blackPlayer, std::vector<Move*> *moves, sf::Font* font);
    void updateMove();
    void updateElo(int whiteElo, int blackElo);
};