#include <SFML/Graphics.hpp>
#include "user.hpp"
#include "partida.hpp"
#include "pieza.hpp"

void renderingThread(sf::RenderWindow* window)
{
    // activate the window's context
    window->setActive(true);

    // the rendering loop
    while (window->isOpen())
    {
        // draw...

        // end the current frame
        window->display();
    }
}

int main()
{
    // // create the window (remember: it's safer to create it in the main thread due to OS limitations)
    // sf::RenderWindow window(sf::VideoMode(800, 600), "OpenGL");

    // // deactivate its OpenGL context
    // window.setActive(false);

    // // launch the rendering thread
    // sf::Thread thread(&renderingThread, &window);
    // thread.launch();

    // // the event/logic/whatever loop
    // while (window.isOpen())
    // {
    //     // check all the window's events that were triggered since the last iteration of the loop
    //     sf::Event event;
    //     while (window.pollEvent(event))
    //     {
    //         // "close requested" event: we close the window
    //         if (event.type == sf::Event::Closed)
    //             window.close();
    //     }

    //     // clear the window with black color
    //     window.clear(sf::Color::Black);

    //     // draw everything here...
    //     // window.draw(...);

    //     // end the current frame
    //     window.display();
    // }

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

    cout<< partida.tablero[7][0] << endl;

    partida.mostrarTablero();

    Pieza* pieza = partida.getPiezaByIndex(2, 0);
    Jugada j = Jugada(pieza, 4, 4);

    cout << "OK" << endl;

    partida.aplicarJugada(&j);

    std::list<Jugada*>::iterator it;
    
    // for (it = partida.getJugadas().begin(); it != partida.getJugadas().end(); ++it)
    // {
    //     std::cout << 
    // }

    for (auto const& i : partida.getJugadas())
    {
        std::cout << "A" <<  i->getNewFile_text() << i->getNewCol() << endl;
    }

    partida.mostrarTablero();
    

    return 0;

}