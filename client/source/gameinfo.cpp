#include "gameinfo.hpp"

GameInfo::GameInfo(User* blancas, User* negras, std::vector<Jugada*>* jugadas, sf::Font* font)
{
    this->jugadas = jugadas;

    container.setOutlineColor(sf::Color::Red);
    container.setFillColor(sf::Color::White);
    container.setPosition(96.0f * 9, 96.0f * 1);
    container.setSize(sf::Vector2f(96.0f * 4, 96.0f * 6));

    nombreBlancas.setFont(*font);
    nombreBlancas.setString(blancas->getUsername());
    nombreBlancas.setCharacterSize(18);
    nombreBlancas.setPosition(96.0f * 10, 96.0f * 6 + 32.0f);
    nombreBlancas.setFillColor(sf::Color::Black);

    std::cout << "blancas: " << blancas->getUsername() << ", negras: " << negras->getUsername() << ". " << blancas->getElo() << " " << negras->getElo() << std::endl;

    nombreNegras.setFont(*font);
    nombreNegras.setString(negras->getUsername());
    nombreNegras.setCharacterSize(18);
    nombreNegras.setPosition(96.0f * 10, 96.0f * 1 + 32.0f);
    nombreNegras.setFillColor(sf::Color::Black);

    eloBlancas.setFont(*font);
    eloBlancas.setString(std::to_string(blancas->getElo()));
    eloBlancas.setCharacterSize(18);
    eloBlancas.setPosition(96.0f * 12, 96.0f * 6 + 32.0f);
    eloBlancas.setFillColor(sf::Color::Green);

    eloNegras.setFont(*font);
    eloNegras.setString(std::to_string(negras->getElo()));
    eloNegras.setCharacterSize(18);
    eloNegras.setPosition(96.0f * 12, 96.0f * 1 + 32.0f);
    eloNegras.setFillColor(sf::Color::Green);
}

void GameInfo::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(container);
    target.draw(nombreBlancas);
    target.draw(nombreNegras);
    target.draw(eloBlancas);
    target.draw(eloNegras);
}

