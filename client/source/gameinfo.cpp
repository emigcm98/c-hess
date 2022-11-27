#include "gameinfo.hpp"

GameInfo::GameInfo(User* blancas, User* negras, std::vector<Jugada*>* jugadas, sf::Font* font)
{
    this->jugadas = jugadas;

    nombreBlancas.setFont(*font);
    nombreBlancas.setString(blancas->getUsername());
    nombreBlancas.setCharacterSize(18);
    nombreBlancas.setPosition(96.0f * 10, 96.0f * 7);
    nombreBlancas.setFillColor(sf::Color::White);

    std::cout << "blancas: " << blancas->getUsername() << ", negras: " << negras->getUsername() << ". " << blancas->getElo() << " " << negras->getElo() << std::endl;

    nombreNegras.setFont(*font);
    nombreNegras.setString(negras->getUsername());
    nombreNegras.setCharacterSize(18);
    nombreNegras.setPosition(96.0f * 10, 96.0f * 1);
    nombreNegras.setFillColor(sf::Color::White);

    eloBlancas.setFont(*font);
    eloBlancas.setString(std::to_string(blancas->getElo()));
    eloBlancas.setCharacterSize(18);
    eloBlancas.setPosition(96.0f * 10, 96.0f * 7 + 24.0f);
    eloBlancas.setFillColor(sf::Color::White);

    eloNegras.setFont(*font);
    eloNegras.setString(std::to_string(negras->getElo()));
    eloNegras.setCharacterSize(18);
    eloNegras.setPosition(96.0f * 10, 96.0f * 1 + 24.0f);
    eloNegras.setFillColor(sf::Color::White);
}

void GameInfo::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(nombreBlancas);
    target.draw(nombreNegras);
    target.draw(eloBlancas);
    target.draw(eloNegras);
}

