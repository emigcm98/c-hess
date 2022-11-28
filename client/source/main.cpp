#include <SFML/Graphics.hpp>
#include "user.hpp"
#include "partida.hpp"
#include "pieza.hpp"

int main()
{
    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf"))
    {
        // error...
        cerr << "No se ha podido cargar la fuente." << endl;
        return 1;
    }

    struct tm birthdate;
    birthdate.tm_year = 1998;
    birthdate.tm_mon = 9; // 0 - 11
    birthdate.tm_mday = 14;

    User u = User("emigcm98", "password", birthdate);
    std::cout << u.calcular_edad() << endl;
    std::cout << u.is_birthday() << endl;
    std::cout << u.getElo() << endl;
    std::cout << u.calculate_new_elo(1340.0, 0.5) << endl;
    std::cout << u.getElo() << endl;

    User u2 = User("fuen", "password", birthdate);

    Partida partida = Partida(&u, &u2, &font);

    // partida.mostrarTablero();

    // Pieza *pieza = partida.getPiezaByPos("e2");
    // Jugada j = Jugada(pieza, "e4");

    // cout << "OK" << endl;

    // partida.aplicarJugada(&j);

    // std::vector<Jugada *>::iterator it;

    // for (auto const &i : partida.getJugadas())
    // {
    //     std::cout << i->to_string() << endl;
    // }

    // create the window (remember: it's safer to create it in the main thread due to OS limitations)
    // sf::RenderWindow window(sf::VideoMode(768,512), "Chess", sf::Style::Titlebar | sf::Style::Close);
    // sf::RenderWindow window(sf::VideoMode(1024,768), "Chess", sf::Style::Titlebar | sf::Style::Close);
    sf::RenderWindow window(sf::VideoMode(96 * 14, 96 * 8), "Chess", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // deactivate its OpenGL context
    window.setActive(false);

    // the event/logic/whatever loop
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if ((0 <= event.mouseButton.x) && (event.mouseButton.x <= (96*8)) && (0 <= event.mouseButton.y) && (event.mouseButton.y <= (96*8)))
                    {
                        unsigned int buttonPos{(event.mouseButton.x / 96) + ( (7-(event.mouseButton.y / 96)) * (8 * ((96*8) / window.getSize().y)))};
                        cout << buttonPos << endl;

                        if (!partida.isSelected())
                        {
                            cout << "nothing selected" << endl;
                            partida.selectPiece(buttonPos);
                        }
                        else
                        {
                            cout << "piece was selected" << endl;
                            partida.moveSelected(buttonPos);
                        }
                            
                    }
                    // else if ((517 <= event.mouseButton.x) && (event.mouseButton.x <= 763) && (5 <= event.mouseButton.y) && (event.mouseButton.y <= 45))
                    // {
                    //     //partida.restart();
                    // }
                }
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        window.draw(partida);

        // end the current frame
        window.display();
    }

    cout << fromChessPosition("e4") << endl;
    cout << toChessPosition(28) << endl;

    return 0;
}