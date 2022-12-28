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
    if (!font.loadFromFile("fonts/Courier-BoldRegular.ttf"))
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
    User u2 = User("fuen", "password", birthdate);

    ChessGame *chessgame = new ChessGame(&u, &u2, &font);

    // IA :)
    ChessAlgorithm *ca = nullptr;
    ca = new RandomChessAlgorithm(chessgame, false);

    // create the window (remember: it's safer to create it in the main thread due to OS limitations)
    sf::RenderWindow window(sf::VideoMode(OBJECT_SIZE * 14, OBJECT_SIZE * 8), "Chess", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // deactivate its OpenGL context
    window.setActive(false);

    // current piece and valid movements
    Piece *p;
    int actualPlay = 0;

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
                //delete chessgame;
                window.close();
            }
            if (!chessgame->isFinished() && event.type == sf::Event::MouseButtonPressed)
            {

                // must be in the actualPlay to select and move!
                if (event.mouseButton.button == sf::Mouse::Left && actualPlay == int(chessgame->getMoves().size()))
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

                        p = chessgame->getSelectedPiece();
                        if (p == nullptr)
                        {
                            chessgame->selectPiece(buttonPos);
                        }
                        else
                        {
                            moved = chessgame->moveSelected(buttonPos);
                            if (moved)
                            {
                                actualPlay++;
                                if (!chessgame->isFinished() && ca != nullptr){
                                    //chessgame->playNextMove();
                                    int mov = ca->getBestOption();
                                    bool moved = chessgame->moveSelected(mov);
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
                    orientation = chessgame->rotateBoard();
                }
                else if (event.key.code == sf::Keyboard::P)
                {
                    std::cout << chessgame->savePgn() << std::endl;
                }
                else if (event.key.code == sf::Keyboard::F)
                {
                    std::cout << chessgame->saveFen() << std::endl;
                }
                else if (event.key.code == sf::Keyboard::Left)
                {
                    if (actualPlay > 0)
                    {
                        actualPlay--;
                        chessgame->undoPlay(actualPlay);
                    }
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    if (actualPlay < int(chessgame->getMoves().size()))
                    {
                        chessgame->applyPlay(actualPlay);
                        actualPlay++;
                    }
                }
            }
            if (chessgame->isFinished()){
                if (ca != nullptr){
                    //delete ca;
                }
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        window.draw(*chessgame);
        window.display();

    }

    return 0;
}