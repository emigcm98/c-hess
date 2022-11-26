#include <SFML/Graphics.hpp>
#include "user.hpp"
#include "partida.hpp"
#include "pieza.hpp"

int main()
{

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

    Partida partida = Partida(&u, &u2);

    //partida.mostrarTablero();

    Pieza* pieza = partida.getPiezaByPos("c1");
    Jugada j = Jugada(pieza, "d4");

    cout << "OK" << endl;

    partida.aplicarJugada(&j);

    std::list<Jugada*>::iterator it;
    
    

    // for (auto const& i : partida.getJugadas())
    // {
    //     std::cout << "A" <<  fromChessPosition(i->getNewPos()) << endl;
    // }

    //partida.mostrarTablero();
    

    // create the window (remember: it's safer to create it in the main thread due to OS limitations)
    sf::RenderWindow window(sf::VideoMode(768,512), "Chess", sf::Style::Titlebar | sf::Style::Close);
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

    cout << fromChessPosition("a1") << endl;
    cout << fromChessPosition("a8") << endl;
    cout << fromChessPosition("d8") << endl;
    cout << fromChessPosition("h8") << endl;

    return 0;
}