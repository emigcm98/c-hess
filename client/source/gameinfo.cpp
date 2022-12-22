#include "gameinfo.hpp"
#include "configuration.cpp"

GameInfo::GameInfo(User *blancas, User *negras, std::vector<Jugada *> *jugadas, sf::Font *font)
{
    this->jugadas = jugadas;

    container.setOutlineColor(sf::Color::Red);
    container.setFillColor(sf::Color::White);
    container.setPosition(OBJECT_SIZE_F * 9, OBJECT_SIZE_F * 1);
    container.setSize(sf::Vector2f(OBJECT_SIZE_F * 4, OBJECT_SIZE_F * 6));

    nombreBlancas.setFont(*font);
    nombreBlancas.setString(blancas->getUsername());
    nombreBlancas.setCharacterSize(18);
    nombreBlancas.setPosition(OBJECT_SIZE_F * 10, OBJECT_SIZE_F * 6 + OBJECT_SIZE_F / 3);
    nombreBlancas.setFillColor(sf::Color::Black);

    // std::cout << "blancas: " << blancas->getUsername() << ", negras: " << negras->getUsername() << ". " << blancas->getElo() << " " << negras->getElo() << std::endl;

    nombreNegras.setFont(*font);
    nombreNegras.setString(negras->getUsername());
    nombreNegras.setCharacterSize(18);
    nombreNegras.setPosition(OBJECT_SIZE_F * 10, OBJECT_SIZE_F * 1 + OBJECT_SIZE_F / 3);
    nombreNegras.setFillColor(sf::Color::Black);

    eloBlancas.setFont(*font);
    eloBlancas.setString(std::to_string(blancas->getElo()));
    eloBlancas.setCharacterSize(18);
    eloBlancas.setPosition(OBJECT_SIZE_F * 12, OBJECT_SIZE_F * 6 + OBJECT_SIZE_F / 3);
    eloBlancas.setFillColor(sf::Color::Green);

    eloNegras.setFont(*font);
    eloNegras.setString(std::to_string(negras->getElo()));
    eloNegras.setCharacterSize(18);
    eloNegras.setPosition(OBJECT_SIZE_F * 12, OBJECT_SIZE_F * 1 + OBJECT_SIZE_F / 3);
    eloNegras.setFillColor(sf::Color::Green);

    lastPlay.setFont(*font);
    lastPlay.setString("NONE");
    lastPlay.setCharacterSize(18);
    lastPlay.setPosition(OBJECT_SIZE_F * 11, OBJECT_SIZE_F * 4);
    lastPlay.setFillColor(sf::Color::Black);
}

void GameInfo::updateJugada()
{
    if (!jugadas->empty()){
        lastPlay.setString(jugadas->back()->to_string());
    }
}

void GameInfo::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(container);
    target.draw(nombreBlancas);
    target.draw(nombreNegras);
    target.draw(eloBlancas);
    target.draw(eloNegras);

    target.draw(lastPlay);
}
