#include <SFML/Graphics.hpp>
#include <chrono>
#include "user.hpp"
#include "partida.hpp"
#include "pieza.hpp"
#include "configuration.cpp"

// by default white
bool orientation = true;

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
    // std::cout << u.calcular_edad() << endl;
    // std::cout << u.is_birthday() << endl;
    // std::cout << u.getElo() << endl;
    // std::cout << u.calculate_new_elo(1340.0, 0.5) << endl;
    // std::cout << u.getElo() << endl;

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
    sf::RenderWindow window(sf::VideoMode(OBJECT_SIZE * 14, OBJECT_SIZE * 8), "Chess", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // deactivate its OpenGL context
    window.setActive(false);

    // current piece and valid movements
    Pieza *p;
    std::vector<int> validMovements;

    // the event/logic/whatever loop
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    // only select in board
                    if ((0 <= event.mouseButton.x) && (event.mouseButton.x <= (OBJECT_SIZE * 8)) && (0 <= event.mouseButton.y) && (event.mouseButton.y <= (OBJECT_SIZE * 8)))
                    {
                        int buttonPos;
                        // unsigned int buttonPos{(event.mouseButton.x / 96) + ((7 - (event.mouseButton.y / 96)) * (8 * ((96 * 8) / window.getSize().y)))};

                        if (orientation)
                        {
                            // cout << "okkk" << endl;
                            buttonPos = (event.mouseButton.x / OBJECT_SIZE) + ((7 - (event.mouseButton.y / OBJECT_SIZE)) * (8 * ((OBJECT_SIZE * 8) / window.getSize().y)));
                        }
                        else
                        {
                            buttonPos = (7 - (event.mouseButton.x / OBJECT_SIZE)) + ((event.mouseButton.y / OBJECT_SIZE) * (8 * ((OBJECT_SIZE * 8) / window.getSize().y)));
                        }
                        // cout << "buttonPos " << buttonPos << endl;
                        p = partida.getSelectedPiece();
                        if (p == nullptr)
                        {
                            validMovements = partida.selectPiece(buttonPos);
                        }
                        else
                        {
                            partida.moveSelected(buttonPos, validMovements);
                        }
                    }
                }
                break;
            case sf::Event::EventType::KeyPressed:

                if (event.key.code == sf::Keyboard::R)
                {
                    partida.rotateBoard();
                    orientation = !orientation;
                }
                break;
            default:
                break;
            }

            // clear the window with black color
            window.clear(sf::Color::Black);

            // draw everything here...
            window.draw(partida);

            // end the current frame
            window.display();
        }
    }

    return 0;
}