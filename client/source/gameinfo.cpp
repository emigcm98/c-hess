#include "gameinfo.hpp"
#include "configuration.cpp"

GameInfo::GameInfo(User *blancas, User *negras, std::vector<Move *> *moves, sf::Font *font)
{
    this->moves = moves;

    container.setOutlineColor(sf::Color::Red);
    container.setFillColor(sf::Color::White);
    container.setPosition(OBJECT_SIZE_F * 9, OBJECT_SIZE_F * 1);
    container.setSize(sf::Vector2f(OBJECT_SIZE_F * 4, OBJECT_SIZE_F * 6));

    whiteName.setFont(*font);
    whiteName.setString(blancas->getUsername());
    whiteName.setCharacterSize(18);
    whiteName.setPosition(OBJECT_SIZE_F * 10, OBJECT_SIZE_F * 6 + OBJECT_SIZE_F / 3);
    whiteName.setFillColor(sf::Color::Black);

    // std::cout << "blancas: " << blancas->getUsername() << ", negras: " << negras->getUsername() << ". " << blancas->getElo() << " " << negras->getElo() << std::endl;

    blackName.setFont(*font);
    blackName.setString(negras->getUsername());
    blackName.setCharacterSize(14);
    blackName.setPosition(OBJECT_SIZE_F * 10, OBJECT_SIZE_F * 1 + OBJECT_SIZE_F / 3);
    blackName.setFillColor(sf::Color::Black);

    whiteElo.setFont(*font);
    whiteElo.setString(std::to_string(blancas->getElo()));
    whiteElo.setCharacterSize(18);
    whiteElo.setPosition(OBJECT_SIZE_F * 12, OBJECT_SIZE_F * 6 + OBJECT_SIZE_F / 3);
    whiteElo.setFillColor(sf::Color::Green);

    blackElo.setFont(*font);
    blackElo.setString(std::to_string(negras->getElo()));
    blackElo.setCharacterSize(18);
    blackElo.setPosition(OBJECT_SIZE_F * 12, OBJECT_SIZE_F * 1 + OBJECT_SIZE_F / 3);
    blackElo.setFillColor(sf::Color::Green);

    lastMove.setFont(*font);
    lastMove.setString("Play :)");
    lastMove.setCharacterSize(18);
    lastMove.setPosition(OBJECT_SIZE_F * 11, OBJECT_SIZE_F * 4);
    lastMove.setFillColor(sf::Color::Black);
}

void GameInfo::updateMove()
{
    if (!moves->empty()){
        lastMove.setString(moves->back()->to_string());
    }
}

void GameInfo::updateElo(int whiteElo, int blackElo)
{
    this->whiteElo.setString(std::to_string(whiteElo));
    this->blackElo.setString(std::to_string(blackElo));
}

void GameInfo::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(container);
    target.draw(whiteName);
    target.draw(blackName);
    target.draw(whiteElo);
    target.draw(blackElo);

    target.draw(lastMove);
}
