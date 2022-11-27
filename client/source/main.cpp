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

    Pieza *pieza = partida.getPiezaByPos("c1");
    Jugada j = Jugada(pieza, "d4");

    cout << "OK" << endl;

    partida.aplicarJugada(&j);

    std::vector<Jugada *>::iterator it;

    for (auto const &i : partida.getJugadas())
    {
        std::cout << i->to_string() << endl;
    }

    // create the window (remember: it's safer to create it in the main thread due to OS limitations)
    // sf::RenderWindow window(sf::VideoMode(768,512), "Chess", sf::Style::Titlebar | sf::Style::Close);
    // sf::RenderWindow window(sf::VideoMode(1024,768), "Chess", sf::Style::Titlebar | sf::Style::Close);
    sf::RenderWindow window(sf::VideoMode(1280, 768), "Chess", sf::Style::Titlebar | sf::Style::Close);
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