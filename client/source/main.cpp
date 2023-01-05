#include "chessgameHandler.hpp"

// by default white
bool orientation = true;

int main()
{
    

    struct tm birthdate;
    birthdate.tm_year = 1998;
    birthdate.tm_mon = 9; // 0 - 11
    birthdate.tm_mday = 14;

    User u = User("emigcm98", "password", birthdate);

    ChessGameHandler cgh = ChessGameHandler(&u, true);
    cgh.start();

    // User u2 = User("fuen", "password", birthdate);

    // ChessGame *chessgame = new ChessGame(&u, &u2, &font);

    // // IA :)
    // ChessAlgorithm *ca = nullptr;
    // ca = new RandomChessAlgorithm(chessgame, false);
    // // ca = new BasicChessAlgorithm(chessgame, false);

    // // create the window (remember: it's safer to create it in the main thread due to OS limitations)
    // sf::RenderWindow window(sf::VideoMode(OBJECT_SIZE * 14, OBJECT_SIZE * 8), "C-hess", sf::Style::Titlebar | sf::Style::Close);
    // window.setVerticalSyncEnabled(true);

    // // deactivate its OpenGL context
    // window.setActive(false);

    // // current piece and valid movements
    // Piece *p;
    // int actualPlay = 0;
    // std::vector<Move *> *gameMoves = chessgame->getMoves();

    // // the event/logic/whatever loop
    // while (window.isOpen())
    // {
    //     // check all the window's events that were triggered since the last iteration of the loop
    //     sf::Event event;
    //     Move *lastMove = nullptr;
    //     bool turn = chessgame->getTurn();
    //     bool selectingPromoted = chessgame->getIfSelectingPromoted();
    //     bool promoting = false;
    //     char pieceNameNotation = '\0';

    //     while (window.pollEvent(event))
    //     {
    //         // "close requested" event: we close the window
    //         if (event.type == sf::Event::Closed)
    //         {
    //             // delete chessgame;
    //             window.close();
    //         }
    //         if (!chessgame->isFinished() && event.type == sf::Event::MouseButtonPressed)
    //         {

    //             // must be in the actualPlay to select and move!
    //             if (event.mouseButton.button == sf::Mouse::Left && actualPlay == int(gameMoves->size()))
    //             {
    //                 // only select in board
    //                 if ((0 <= event.mouseButton.x) && (event.mouseButton.x <= (OBJECT_SIZE * 8)) && (0 <= event.mouseButton.y) && (event.mouseButton.y <= (OBJECT_SIZE * 8)))
    //                 {
    //                     int buttonPos;

    //                     if (orientation)
    //                     {
    //                         buttonPos = (event.mouseButton.x / OBJECT_SIZE) + ((7 - (event.mouseButton.y / OBJECT_SIZE)) * (8 * ((OBJECT_SIZE * 8) / window.getSize().y)));
    //                     }
    //                     else
    //                     {
    //                         buttonPos = (7 - (event.mouseButton.x / OBJECT_SIZE)) + ((event.mouseButton.y / OBJECT_SIZE) * (8 * ((OBJECT_SIZE * 8) / window.getSize().y)));
    //                     }
    //                     // buttonPos = (event.mouseButton.x / OBJECT_SIZE) + ((7 - (event.mouseButton.y / OBJECT_SIZE)) * (8 * ((OBJECT_SIZE * 8) / window.getSize().y)));

    //                     // wait for user input to select the type of piece to promote
    //                     if (selectingPromoted)
    //                     {
    //                         lastMove = gameMoves->back();
    //                         if (buttonPos == lastMove->getNewPos() || buttonPos == lastMove->getNewPos() - 8 ||
    //                             buttonPos == lastMove->getNewPos() - 16 || buttonPos == lastMove->getNewPos() - 24)
    //                         {
    //                             pieceNameNotation = chessgame->getPieceType(7-buttonPos/8);
    //                             promoting = true;
    //                         }
    //                         break;
    //                     }

    //                     if (turn) // player turn
    //                     {
    //                         p = chessgame->getSelectedPiece();
    //                         if (p == nullptr)
    //                         {
    //                             chessgame->selectPiece(buttonPos);
    //                         }
    //                         else
    //                         {
    //                             lastMove = chessgame->moveSelected(buttonPos);
    //                             if (lastMove != nullptr)
    //                             {
    //                                 actualPlay++;
    //                             }
    //                             else {
    //                                 selectingPromoted = chessgame->getIfSelectingPromoted();
    //                                 if (selectingPromoted){
    //                                     actualPlay++;
    //                                     std::cout << "lastMove == null == promoted!!" << std::endl;
    //                                 }
                                    
    //                             }
    //                             // else { // can be promoted
    //                             //     if (selectingPromoted){ // user is selecting promoted after pawn arrived to last row

    //                             //     }
    //                             // }
    //                         }
    //                     }
    //                 }
    //             }
    //         }
    //         if (event.type == sf::Event::EventType::KeyPressed)
    //         {

    //             if (event.key.code == sf::Keyboard::R)
    //             {
    //                 orientation = chessgame->rotateBoard();
    //             }
    //             else if (event.key.code == sf::Keyboard::P)
    //             {
    //                 std::cout << chessgame->savePgn() << std::endl;
    //             }
    //             else if (event.key.code == sf::Keyboard::F)
    //             {
    //                 std::cout << chessgame->saveFen() << std::endl;
    //             }
    //             else if (event.key.code == sf::Keyboard::Left)
    //             {
    //                 if (actualPlay > 0)
    //                 {
    //                     actualPlay--;
    //                     chessgame->undoPlay(actualPlay);
    //                 }
    //             }
    //             else if (event.key.code == sf::Keyboard::Right)
    //             {
    //                 if (actualPlay < int(gameMoves->size()))
    //                 {
    //                     chessgame->applyPlay(actualPlay);
    //                     actualPlay++;
    //                 }
    //             }
    //             else if (event.key.code == sf::Keyboard::Escape)
    //             {
    //                 if (actualPlay > 0)
    //                 {
    //                     actualPlay--;
    //                     // chessgame->takeback();
    //                     chessgame->undoPlay();
    //                 }
    //             }
    //             else if (event.key.code == sf::Keyboard::J)
    //             {
    //                 std::cout << gameMoves->size() << std::endl;
    //             }
    //         }
    //         if (chessgame->isFinished())
    //         {
    //             if (ca != nullptr)
    //             {
    //                 // delete ca;
    //             }
    //         }
    //     }

    //     // AI turn! (!turn)
    //     if (!turn && !chessgame->isFinished() && ca != nullptr)
    //     {
    //         // chess engine
    //         int mov = ca->getBestOption();
    //         // wait a little time
    //         sf::sleep(sf::milliseconds(500));
    //         Move *lastMove2 = chessgame->moveSelected(mov);
    //         if (lastMove2 != nullptr)
    //         {
    //             actualPlay++;
    //         }
    //     }

    //     if (promoting)
    //     {

    //         // while(window.pollEvent()){
    //         // }
    //         std::cout << "selecting promoted!!" << std::endl;
    //         selectingPromoted = false;

    //         if (pieceNameNotation != 0)
    //         {
    //             chessgame->promote(lastMove, pieceNameNotation);
    //             //?
    //             promoting = false;
    //             //actualPlay++;
    //         }
    //     }

    //     // clear the window with black color
    //     window.clear(sf::Color::Black);

    //     window.draw(*chessgame);

    //     window.display();

    // }

    return 0;
}