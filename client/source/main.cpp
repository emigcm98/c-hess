#include <SFML/Graphics.hpp>
#include <chrono>
#include "user.hpp"
#include "piece.hpp"
#include "configuration.cpp"
#include "algorithm.hpp"

// by default white
bool orientation = true;

int main()
{
    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf"))
    {
        // error...
        std::cerr << "No se ha podido cargar la fuente." << std::endl;
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

    ChessGame partida = ChessGame(&u, &u2, &font);

    // IA :)
    RandomChessAlgorithm ca = RandomChessAlgorithm(&partida, false);

    // create the window (remember: it's safer to create it in the main thread due to OS limitations)
    sf::RenderWindow window(sf::VideoMode(OBJECT_SIZE * 14, OBJECT_SIZE * 8), "Chess", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // deactivate its OpenGL context
    window.setActive(false);

    // current piece and valid movements
    Piece *p;
    // std::vector<int> validMovements;
    int actualPlay = 0;

    // Alfil *aux = Pieza::create(10, 'A');
    // std::cout << "creando pieza. Es de tipo alfil?" << std::endl;
    // std::cout << instanceof<Alfil>(aux) << std::endl;

    // the event/logic/whatever loop
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        bool moved = false;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (!partida.isFinished() && event.type == sf::Event::MouseButtonPressed)
            {

                // must be in the actualPlay to select and move!
                if (event.mouseButton.button == sf::Mouse::Left && actualPlay == int(partida.getMoves().size()))
                {

                    // only select in board
                    if ((0 <= event.mouseButton.x) && (event.mouseButton.x <= (OBJECT_SIZE * 8)) && (0 <= event.mouseButton.y) && (event.mouseButton.y <= (OBJECT_SIZE * 8)))
                    {
                        int buttonPos;

                        if (orientation)
                        {
                            buttonPos = (event.mouseButton.x / OBJECT_SIZE) + ((7 - (event.mouseButton.y / OBJECT_SIZE)) * (8 * ((OBJECT_SIZE * 8) / window.getSize().y)));
                        }
                        else
                        {
                            buttonPos = (7 - (event.mouseButton.x / OBJECT_SIZE)) + ((event.mouseButton.y / OBJECT_SIZE) * (8 * ((OBJECT_SIZE * 8) / window.getSize().y)));
                        }
                        // buttonPos = (event.mouseButton.x / OBJECT_SIZE) + ((7 - (event.mouseButton.y / OBJECT_SIZE)) * (8 * ((OBJECT_SIZE * 8) / window.getSize().y)));

                        p = partida.getSelectedPiece();
                        if (p == nullptr)
                        {
                            partida.selectPiece(buttonPos);
                        }
                        else
                        {
                            moved = partida.moveSelected(buttonPos);
                            if (moved)
                            {
                                actualPlay++;
                                if (!partida.isFinished()){
                                    //partida.playNextMove();
                                    int mov = ca.getBestOption();
                                    std::cout << "mov: " << mov << std::endl;
                                    bool moved = partida.moveSelected(mov);
                                    if (moved)
                                    {
                                        actualPlay++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (event.type == sf::Event::EventType::KeyPressed)
            {

                if (event.key.code == sf::Keyboard::R)
                {
                    orientation = partida.rotateBoard();
                }
                else if (event.key.code == sf::Keyboard::P)
                {
                    std::cout << partida.savePgn() << std::endl;
                }
                else if (event.key.code == sf::Keyboard::F)
                {
                    std::cout << partida.saveFen() << std::endl;
                }
                else if (event.key.code == sf::Keyboard::Left)
                {
                    if (actualPlay > 0)
                    {
                        actualPlay--;
                        partida.undoPlay(actualPlay);
                    }
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    if (actualPlay < int(partida.getMoves().size()))
                    {
                        partida.applyPlay(actualPlay);
                        actualPlay++;
                    }
                }
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        window.draw(partida);
        window.display();

    }

    return 0;
}