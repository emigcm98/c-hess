#include <SFML/Graphics.hpp>
#include "chessgameHandler.hpp"

ChessGameHandler::ChessGameHandler(User *player, User *player2, bool humanColor)
{
    if (!font.loadFromFile("fonts/Courier-BoldRegular.ttf"))
    {
        // error...
        std::cerr << "Failed to load font." << std::endl;
        return;
    }

    this->humanColor = humanColor;
    this->player = player;
    this->player2 = player2;

    struct tm birthdate;
    birthdate.tm_year = 1998;
    birthdate.tm_mon = 9; // 0 - 11
    birthdate.tm_mday = 14;

    if (this->player2 == nullptr)
    {
        // computer
        this->player2 = new User("randomChess", "password", birthdate, 500.0);

        this->chessgame = new ChessGame(this->player, this->player2, &font);
        this->ca = new RandomChessAlgorithm(chessgame, !humanColor);
        // this->ca = new BasicChessAlgorithm(chessgame, !humanColor);
    }
    else
    {
        this->chessgame = new ChessGame(this->player, this->player2, &font);
        this->ca = nullptr;
    }

    this->orientation = true;
    this->turn = this->chessgame->getTurn();

    // create the window (remember: it's safer to create it in the main thread due to OS limitations)
    window.create(sf::VideoMode(OBJECT_SIZE * 14, OBJECT_SIZE * 8), "C-hess", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // deactivate its OpenGL context
    window.setActive(false);

    // current piece and valid movements
    this->selectedPiece = nullptr;
    this->nMove = 0;
    this->gameMoves = chessgame->getMoves();
}

ChessGameHandler::~ChessGameHandler()
{
    // std::cout << "deleting ChessAlgorithm:" << std::endl;
    delete ca;
    // std::cout << "deleting ChessGame:" << std::endl;
    delete chessgame;
}

int ChessGameHandler::start()
{
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        Move *lastMove = nullptr;
        // bool* turn = chessgame->getTurn();
        bool selectingPromoted = chessgame->getIfSelectingPromoted();
        bool promoting = false;
        char pieceNameNotation = '\0';

        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
            {
                // delete chessgame->
                window.close();
            }
            if (!chessgame->isFinished() && event.type == sf::Event::MouseButtonPressed)
            {

                // must be in the nMove to select and move!
                if (event.mouseButton.button == sf::Mouse::Left && nMove == int(gameMoves->size()))
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

                        // wait for user input to select the type of piece to promote

                        if (*turn == humanColor || ca == nullptr) // player turn
                        {
                            if (selectingPromoted)
                            {
                                lastMove = gameMoves->back();
                                if (*turn)
                                {
                                    if (buttonPos == lastMove->getNewPos() || buttonPos == lastMove->getNewPos() - 8 ||
                                        buttonPos == lastMove->getNewPos() - 16 || buttonPos == lastMove->getNewPos() - 24)
                                    {
                                        pieceNameNotation = chessgame->getPieceType(7 - buttonPos / 8);
                                        promoting = true;
                                    }
                                }
                                else {
                                    std::cout << "buttonPos: " << buttonPos << std::endl;
                                    std::cout << "lastMoveNewPos: " << lastMove->getNewPos() << std::endl;
                                    if (buttonPos == lastMove->getNewPos() || buttonPos == lastMove->getNewPos() + 8 ||
                                        buttonPos == lastMove->getNewPos() + 16 || buttonPos == lastMove->getNewPos() + 24)
                                    {
                                        pieceNameNotation = chessgame->getPieceType(buttonPos / 8);
                                        promoting = true;
                                    }
                                }

                                break;
                            }

                            selectedPiece = chessgame->getSelectedPiece();
                            if (selectedPiece == nullptr)
                            {
                                chessgame->selectPiece(buttonPos);
                            }
                            else
                            {
                                lastMove = chessgame->moveSelected(buttonPos);
                                if (lastMove != nullptr)
                                {
                                    nMove++;
                                }
                                else
                                {
                                    selectingPromoted = chessgame->getIfSelectingPromoted();
                                    if (selectingPromoted)
                                    {
                                        nMove++;
                                        std::cout << "lastMove == null == promoted!!" << std::endl;
                                    }
                                }
                                // else { // can be promoted
                                //     if (selectingPromoted){ // user is selecting promoted after pawn arrived to last row

                                //     }
                                // }
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
                    if (nMove > 0)
                    {
                        nMove--;
                        chessgame->undoMove(nMove);
                    }
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    if (nMove < int(gameMoves->size()))
                    {
                        chessgame->applyPlay(nMove);
                        nMove++;
                    }
                }
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    if (nMove > 1)
                    {
                        nMove--;
                        // chessgame->takeback();
                        chessgame->undoMove();
                        nMove--;
                        chessgame->undoMove();
                    }
                }
                else if (event.key.code == sf::Keyboard::F10)
                {
                    window.close();
                    return 1;
                }
                else if (event.key.code == sf::Keyboard::W)
                {
                    std::cout << "White Pieces: " << std::endl;
                    for (auto const &i : *chessgame->getWhitePieces())
                    {
                        std::cout << i->getNameFEN() << " (" << toChessPosition(i->getPos()) << ") [times=" << i->getTimesMoved() << "]" << std::endl;
                    }
                    std::cout << "White Pieces Killed: " << std::endl;
                    for (auto const &i : *chessgame->getWhitePiecesKilled())
                    {
                        std::cout << i->getNameFEN() << " (" << toChessPosition(i->getPos()) << ") [times=" << i->getTimesMoved() << "]" << std::endl;
                    }
                    std::cout << std::endl;
                }
                else if (event.key.code == sf::Keyboard::B)
                {
                    std::cout << "Black Pieces: " << std::endl;
                    for (auto const &i : *chessgame->getBlackPieces())
                    {
                        std::cout << i->getNameFEN() << " (" << toChessPosition(i->getPos()) << ") [times=" << i->getTimesMoved() << "]" << std::endl;
                    }
                    std::cout << "Black Pieces Killed: " << std::endl;
                    for (auto const &i : *chessgame->getBlackPiecesKilled())
                    {
                        std::cout << i->getNameFEN() << " (" << toChessPosition(i->getPos()) << ") [times=" << i->getTimesMoved() << "]" << std::endl;
                    }
                    std::cout << std::endl;
                }
                // else if (event.key.code == sf::Keyboard::J)
                // {
                //     std::cout << gameMoves->size() << std::endl;
                // }
            }
        }

        // AI turn! (!turn)
        if (*turn != humanColor && !chessgame->isFinished() && ca != nullptr)
        {
            // chess engine
            int mov = ca->getBestOption();
            // wait a little time
            sf::sleep(sf::milliseconds(500));

            Move *lastMove2 = chessgame->moveSelected(mov, false);
            if (lastMove2 != nullptr)
            {
                nMove++;
            }

            selectingPromoted = chessgame->getIfSelectingPromoted();
            if (selectingPromoted)
            {
                lastMove2 = gameMoves->back();
                chessgame->promote(lastMove2, turn ? 'Q' : 'q');
                promoting = false;
                nMove++;
            }
        }

        if ((*turn == humanColor || ca == nullptr) && promoting)
        {

            // while(window.pollEvent()){
            // }
            std::cout << "selecting promoted!!" << std::endl;
            selectingPromoted = false;

            if (pieceNameNotation != 0)
            {
                chessgame->promote(lastMove, pieceNameNotation);
                //?
                promoting = false;
                // nMove++;
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        window.draw(*chessgame);

        window.display();
    }

    return 0;
}
