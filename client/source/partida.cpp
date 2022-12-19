#include "partida.hpp"
#include "configuration.cpp"

template <typename Base, typename T>
inline bool instanceof (const T *ptr)
{
    return dynamic_cast<const Base *>(ptr) != nullptr;
}

Partida::Partida(User *usuario_blancas, User *usuario_negras, sf::Font *font)
{
    this->usuario_blancas = usuario_blancas;
    this->usuario_negras = usuario_negras;
    this->fecha = time(0);

    this->gameInfo = new GameInfo(usuario_blancas, usuario_negras, &jugadas, font);

    turn = true;
    selected = false;
    selectedPiece = nullptr;
    potentiallyPieceEnPassant = nullptr;

    orientation = true;

    // tablero vacio
    for (int i = 0; i < 64; i++)
    {
        tablero[i] = nullptr;
    }

    // peones

    piezas_blanco.push_back(tablero[fromChessPosition("a2")] = new Peon("a2", true));
    piezas_blanco.push_back(tablero[fromChessPosition("b2")] = new Peon("b2", true));
    piezas_blanco.push_back(tablero[fromChessPosition("c2")] = new Peon("c2", true));
    piezas_blanco.push_back(tablero[fromChessPosition("d2")] = new Peon("d2", true));
    piezas_blanco.push_back(tablero[fromChessPosition("e2")] = new Peon("e2", true));
    piezas_blanco.push_back(tablero[fromChessPosition("f2")] = new Peon("f2", true));
    piezas_blanco.push_back(tablero[fromChessPosition("g2")] = new Peon("g2", true));
    piezas_blanco.push_back(tablero[fromChessPosition("h2")] = new Peon("h2", true));
    piezas_negro.push_back(tablero[fromChessPosition("a7")] = new Peon("a7", false));
    piezas_negro.push_back(tablero[fromChessPosition("b7")] = new Peon("b7", false));
    piezas_negro.push_back(tablero[fromChessPosition("c7")] = new Peon("c7", false));
    piezas_negro.push_back(tablero[fromChessPosition("d7")] = new Peon("d7", false));
    piezas_negro.push_back(tablero[fromChessPosition("e7")] = new Peon("e7", false));
    piezas_negro.push_back(tablero[fromChessPosition("f7")] = new Peon("f7", false));
    piezas_negro.push_back(tablero[fromChessPosition("g7")] = new Peon("g7", false));
    piezas_negro.push_back(tablero[fromChessPosition("h7")] = new Peon("h7", false));

    // alfiles
    piezas_blanco.push_back(tablero[fromChessPosition("c1")] = new Alfil("c1", true));
    piezas_blanco.push_back(tablero[fromChessPosition("f1")] = new Alfil("f1", true));
    piezas_negro.push_back(tablero[fromChessPosition("c8")] = new Alfil("c8", false));
    piezas_negro.push_back(tablero[fromChessPosition("f8")] = new Alfil("f8", false));

    // caballos
    piezas_blanco.push_back(tablero[fromChessPosition("b1")] = new Caballo("b1", true));
    piezas_blanco.push_back(tablero[fromChessPosition("g1")] = new Caballo("g1", true));
    piezas_negro.push_back(tablero[fromChessPosition("b8")] = new Caballo("b8", false));
    piezas_negro.push_back(tablero[fromChessPosition("g8")] = new Caballo("g8", false));

    // torres
    piezas_blanco.push_back(tablero[fromChessPosition("a1")] = new Torre("a1", true));
    piezas_blanco.push_back(tablero[fromChessPosition("h1")] = new Torre("h1", true));
    piezas_negro.push_back(tablero[fromChessPosition("a8")] = new Torre("a8", false));
    piezas_negro.push_back(tablero[fromChessPosition("h8")] = new Torre("h8", false));

    // rey
    piezas_blanco.push_back(tablero[fromChessPosition("e1")] = new Rey("e1", true));
    piezas_negro.push_back(tablero[fromChessPosition("e8")] = new Rey("e8", false));

    // dama
    piezas_blanco.push_back(tablero[fromChessPosition("d1")] = new Dama("d1", true));
    piezas_negro.push_back(tablero[fromChessPosition("d8")] = new Dama("d8", false));

    load();
}

void Partida::load(sf::Color col1, sf::Color col2)
{
    for (int i = 0; i < 8; i++)
    {

        bool tmpColor = ((i % 2) == 0) ? true : false;

        for (int j = 0; j < 8; j++)
        {

            m_boardSquares[j + (i * 8)].setPosition(sf::Vector2f(j * OBJECT_SIZE_F, i * OBJECT_SIZE_F));
            m_boardSquares[j + (i * 8)].setSize(sf::Vector2f(OBJECT_SIZE_F, OBJECT_SIZE_F));
            m_boardSquares[j + (i * 8)].setFillColor(tmpColor ? col1 : col2);

            tmpColor = !tmpColor;
        }
    }
}

void Partida::draw(sf::RenderTarget &target, sf::RenderStates states) const
{

    target.clear(sf::Color(128, 128, 128, 0));

    for (int i = 0; i < 64; i++)
    {
        target.draw(m_boardSquares[i]);
    }

    for (int i = 0; i < 16; i++)
    {
        target.draw(*piezas_blanco[i]);
        target.draw(*piezas_negro[i]);
    }

    if (selectedPiece != nullptr && selected)
    {
        for (auto const &i : possibleMovesSquares)
        {
            target.draw(i);
        }
    }

    target.draw(*gameInfo);
}

// aux
// int fromChessPosition(std::string cp)
// {
//     // a1
//     if (cp.size() != 2)
//         return -1;
//     int row = cp.at(1) - 1;
//     int col = cp.at(0) - 97;

//     return row*col + row;
// }

std::vector<int> Partida::filterValidMovements(Pieza *p)
{

    int pos = p->getPos();
    std::vector<int> movements = p->calcularMovimiento();

    //return movements; // PRUEBA

    // std::cout << "movements: ";
    // for (auto m : movements)
    // {
    //     std::cout << m << " ";
    // }
    // std::cout << std::endl;

    // int size = (int)movements.size();

    for (auto it = begin(movements); it != end(movements);)
    {
        bool isSameColor = tablero[*it] != nullptr && (tablero[*it]->getColor() == p->getColor());
        // check if same color pieces
        if (tablero[*it] != nullptr && isSameColor)
        {
            int div;
            if (abs(*it / 8 - pos / 8) == 0)
            {
                div = 1;
            }
            else
            {
                div = abs(*it / 8 - pos / 8);
            }
            int diff = abs(*it - pos) / div;

            int firstSquare = *it;
            std::cout << "hay una pieza en la pos " << firstSquare << std::endl;

            // if (tablero[*it]->getColor() == p->getColor()){
            //     movements.erase(it);
            // }
            movements.erase(it);

            // size--;
            std::cout << "diff: " << diff << std::endl;

            int deleted = 0;

            // for (int i = 0; i < (size-1); i++) {

            if (! instanceof <Rey>(p))
            {
                int i = 1;
                while (it != end(movements) && i < 7)
                {
                    // if ((*(it) % diff) == (pos))
                    std::cout << "fs: " << firstSquare << ", *it: " << *it << ", i: " << i << std::endl;
                    if (firstSquare == (*it - i * diff))
                    {
                        std::cout << "borrando! " << *(it) << std::endl;
                        movements.erase(it);
                        // size--;
                        deleted++;
                        // i--;
                    }
                    else
                    {
                        std::cout << *(it) << " y consecutivos es valido" << std::endl;
                        break;
                    }
                    i++;
                }
                std::cout << "breaking" << std::endl;
            }
            else // i'm the king
            {
                // enroque
                // check if check ?

                // if (!p->getMoved()) // no ha movido el rey
                // {
                //     // enroque corto
                //     if ((*it - pos) == 2)
                //     {
                //         if (tablero[pos + 1] == nullptr && tablero[pos + 2] == nullptr && instanceof<Torre>(tablero[pos + 3]) && !tablero[pos + 3]->getMoved())
                //         {

                //         }
                //         else{
                //             movements.erase(it);
                //         }
                //     }

                //     // enroque largo
                //     if ((*it - pos) == -2)
                //     {
                //         if (tablero[pos - 1] == nullptr && tablero[pos - 2] == nullptr && tablero[pos - 3] == nullptr && instanceof<Torre>(tablero[pos - 4]) && !tablero[pos - 4]->getMoved())
                //         {

                //         }
                //         else{
                //             movements.erase(it);
                //         }
                //     }
                // }
            }

            // it+=deleted;
        }
        // different color pieces
        else if (tablero[*it] != nullptr && !isSameColor)
        {
            if (instanceof <Peon>(p))
            {
                // in front of, cannot eat
                if (abs(*it - pos) == 8)
                {
                    movements.erase(it);
                    continue;
                }
                ++it;
            }
        }
        else // no piece!
        {
            if (instanceof <Peon>(p))
            {
                if (abs(*it - pos) == 7)
                {
                    if (potentiallyPieceEnPassant != nullptr && tablero[pos - 1] == potentiallyPieceEnPassant)
                    {
                    }
                    else
                    {
                        movements.erase(it);
                        continue;
                    }
                }
                else if (abs(*it - pos) == 9)
                {
                    if (potentiallyPieceEnPassant && tablero[pos + 1] == potentiallyPieceEnPassant)
                    {
                    }
                    else
                    {
                        movements.erase(it);
                        continue;
                    }
                }
            }
            ++it;
        }
    }

    // std::cout << "new movements: ";
    // for (auto m : movements)
    // {
    //     std::cout << m << " ";
    // }
    // std::cout << std::endl;

    return movements;
}

bool Partida::aplicarJugada(Jugada *j, std::vector<int> movements)
{

    // se mira si se puede hacer (la Piezq tiene movimeinto, no hay jaque)

    // si se puede hacer, se mueve, se actualiza el tablero y se devuelve true (y se mete a la lista de jugadas)

    bool is_aplicable = false;

    Pieza *pieza = j->getPieza();
    int newpos = j->getNewPos();

    bool enPassant = false;

    if (potentiallyPieceEnPassant != nullptr)
    {
        if (abs(pieza->getPos() - newpos) == 7 || abs(pieza->getPos() - newpos) == 9)
        {
            enPassant = true;
        }
    }

    //std::vector<int> movements = filterValidMovements(pieza);

    bool possibleShortCastling = false;
    bool possibleLongCastling = false;

    // newpos avalaible, movements bigger than 1
    if (pieza->getPos() == j->getNewPos()) // same square, no movement
    {
        selectedPiece = nullptr;
        selected = false;
    }
    else if (std::count(movements.begin(), movements.end(), newpos))
    {
        selectedPiece = nullptr;
        selected = false;
        is_aplicable = true;

        if (instanceof <Rey>(pieza))
        {
            if (newpos - pieza->getPos() == 2) // enroque corto
            {
                possibleShortCastling = true;
            }
            else if (newpos - pieza->getPos() == -2) // enroque largo
            {
                possibleLongCastling = true;
            }
        }
    }

    if (is_aplicable)
    {
        jugadas.push_back(j);

        // quitamos movimiento del antiguo
        tablero[pieza->getPos()] = nullptr;

        // si ya habia una pieza, se come y se elimina
        int enemyPos;
        if (enPassant)
        {
            enemyPos = newpos - 8;
            std::cout << "enemyPos: " << enemyPos << std::endl;
        }
        else
        {
            enemyPos = newpos;
        }
        Pieza *pieza_enemiga = tablero[enemyPos];

        if (pieza_enemiga != nullptr && pieza_enemiga->getColor() != pieza->getColor())
        {
            if (pieza_enemiga->getColor()) // blanca
            {
                for (int i = 0; i < 16; i++)
                {
                    if (piezas_blanco[i]->getPos() == enemyPos)
                    {
                        piezas_blanco.erase(piezas_blanco.begin() + i);
                    }
                }
            }
            else
            {
                for (int i = 0; i < 16; i++)
                {
                    if (piezas_negro[i]->getPos() == enemyPos)
                    {
                        piezas_negro.erase(piezas_negro.begin() + i);
                    }
                }
            }
            delete tablero[enemyPos];
        }

        // for (auto const &i : piezas_blanco)
        // {
        //     cout<<i->getNombre()<<endl;
        // }
        // for (auto const &i : piezas_negro)
        // {
        //     cout<<i->getNombre()<<endl;
        // }

        if (possibleShortCastling)
        {
            Pieza *rook = tablero[pieza->getPos() + 3];
            rook->move(pieza->getPos() + 1);
            tablero[pieza->getPos() + 3] = nullptr;
            tablero[pieza->getPos() + 1] = rook;
            j->shortCastle(); // in the play
            shortCastle(); // in the game itself
            // 0-0
        }
        if (possibleLongCastling)
        {

            Pieza *rook = tablero[pieza->getPos() - 4];
            rook->move(pieza->getPos() - 1);
            tablero[pieza->getPos() - 4] = nullptr;
            tablero[pieza->getPos() - 1] = rook;
            j->longCastle(); // in the play
            longCastle(); // in the game itself
            // 0-0-0
        }

        // movemos la pieza
        pieza->move(newpos);
        // ponemos el nuevo
        tablero[newpos] = pieza;
    }

    // si no se puede hacer se devuelve false y no se le pasa el turno al otro jugador

    potentiallyPieceEnPassant = nullptr;

    return is_aplicable;
}

std::vector<int> Partida::selectPiece(int pos)
{

    // std::cout << "selected piece: " << selectedPiece->getPos() << std::endl;
    // std::cout << "new selected piece: " << pos << std::endl;

    // if (selectedPiece->getPos() == pos)
    // {
    //     std::cout << "es la misma!!" << std::endl;
    //     selectedPiece = nullptr;
    //     selected = false;
    //     return false;
    // }

    std::vector<int> validMovements;

    for (int i = 0; i < 16; i++)
    {
        if (turn)
        {
            if (piezas_blanco[i]->getPos() == pos)
            {
                selectedPiece = piezas_blanco[i];
                selected = true;
                break;
            }
        }
        else
        {
            if (piezas_negro[i]->getPos() == pos)
            {
                selectedPiece = piezas_negro[i];
                selected = true;
                break;
            }
        }
        selected = false;
    }

    if (!selected)
    {
        selectedPiece = nullptr;
        possibleMovesSquares.clear();
        return validMovements;
    }

    if (selectedPiece != nullptr)
    {

        potentiallyPieceEnPassant = nullptr;

        if (instanceof <Peon>(selectedPiece) && jugadas.size() > 0)
        {

            Jugada *previousPlay = jugadas.back();
            Pieza *previousPiece = previousPlay->getPieza();

            if (instanceof <Peon>(previousPiece) && previousPiece->getColor() != selectedPiece->getColor() && previousPlay->isFirstPieceMove())
            {
                if ((previousPiece->getPos() - selectedPiece->getPos()) == 1)
                {
                    potentiallyPieceEnPassant = previousPiece;
                }
                else if ((previousPiece->getPos() - selectedPiece->getPos()) == -1)
                {
                    potentiallyPieceEnPassant = previousPiece;
                }
            }
        }
    }

    // movimientos disponibles
    validMovements = createMovesSquares();

    return validMovements;
}

bool Partida::isSelected()
{
    return selected;
}

std::vector<int> Partida::createMovesSquares()
{

    std::vector<int> validMovements = filterValidMovements(selectedPiece);
    
    if (selectedPiece == nullptr)
        return validMovements;

    possibleMovesSquares.clear();

    for (int i = 0; i < (int)validMovements.size(); i++)
    {
        sf::RectangleShape tmp;
        tmp.setPosition(sf::Vector2f((validMovements.at(i) % 8) * OBJECT_SIZE_F, (7 - (validMovements.at(i) / 8)) * OBJECT_SIZE_F));
        tmp.setSize(sf::Vector2f(OBJECT_SIZE_F, OBJECT_SIZE_F));
        if (tablero[validMovements.at(i)] != nullptr && tablero[validMovements.at(i)]->getColor() != selectedPiece->getColor()) // another piece, put in red
        {
            // red
            tmp.setFillColor(sf::Color(0xff000050));
        }
        else
        {
            //blue
            tmp.setFillColor(sf::Color(0x66b4cc50));
        }

        possibleMovesSquares.push_back(tmp);
    }

    sf::RectangleShape tmp;
    tmp.setPosition(sf::Vector2f((selectedPiece->getPos() % 8) * OBJECT_SIZE_F, (7 - (selectedPiece->getPos() / 8)) * OBJECT_SIZE_F));
    tmp.setSize(sf::Vector2f(OBJECT_SIZE_F, OBJECT_SIZE_F));
    tmp.setFillColor(sf::Color(0x00000000));
    tmp.setOutlineColor(sf::Color::Red);
    tmp.setOutlineThickness(-4.f);
    possibleMovesSquares.push_back(tmp);

    return validMovements;
}

// void Partida::mostrarTablero()
// {
//     for (int j = 0; j < 8; j++)
//     {
//         for (int i = 0; i < 8; i++)
//         {
//             if (tablero[i][j] == nullptr)
//             {
//                 cout << "X"
//                      << " ";
//             }
//             else
//             {
//                 // tablero[i][j]->getNombre()
//                 cout << tablero[i][j]->getNombre() << " ";
//             }
//         }
//         cout << endl;
//     }
//     cout << endl;
// }

void Partida::moveSelected(int pos, std::vector<int> validMovements)
{

    // no piece or not selected
    if ((selectedPiece == nullptr) || !selected)
        return;

    // same piece or no movements
    if ((selectedPiece != nullptr && selectedPiece->getPos() == pos))
    {
        selectedPiece = nullptr;
        selected = false;
        return;
    }
    // another piece
    else if (selectedPiece != nullptr && tablero[pos] != nullptr && tablero[pos]->getColor() == selectedPiece->getColor())
    {
        selectPiece(pos);
        return;
    }
    else if (validMovements.empty())
    {
        return;
    }

    // if everything ok, new play is created
    Jugada *j = new Jugada(selectedPiece, pos);

    bool pieceHasMoved = selectedPiece->getMoved();

    bool valid = aplicarJugada(j, validMovements);

    if (valid)
    {
        turn = !turn;
        // si no ha movido antes ponemos qeue esta jugada es la primera en la que mueve
        if (!pieceHasMoved)
        {
            j->firstPieceMoved(); // ponemos que ha sido el primer movimiento de la pieza
        }
        gameInfo->updateJugada();
    }

    //     lastMove = "Last Turn:\n" + selectedPiece->toString();
    //     for(int i=0; i<16; i++){
    //         if(selectedPiece->getPlayer()){ // If White
    //             if(blackPieces[i].getPosition() == pos){
    //                 blackPieces[i].setPosition(-1);
    //                 break;
    //             }
    //         }
    //         else{ // If Black
    //             if(whitePieces[i].getPosition() == pos){
    //                 whitePieces[i].setPosition(-1);
    //                 break;
    //             }
    //         }
    //     }

    //     if(playerTurnCheck){
    //         playerTurnCheck = false;
    //     }

    //     playerTurn = !playerTurn; // Here player turn changes
    //     calcPossibleMoves();
    // }
}

void Partida::rotateBoard()
{
    orientation = !orientation;
    for (auto const &i : piezas_blanco)
    {
        i->rotate(orientation);
    }
    for (auto const &i : piezas_negro)
    {
        i->rotate(orientation);
    }
}

void Partida::setResultado(Resultado r)
{
    usuario_blancas->calculate_new_elo(usuario_negras->getElo(), r);
    usuario_negras->calculate_new_elo(usuario_blancas->getElo(), r);
}

bool Partida::isJaque()
{
    return false;
}

bool Partida::isJaqueMate()
{
    return false;
}

Pieza *Partida::getPiezaByPos(std::string pos)
{
    return tablero[fromChessPosition(pos)];
}

vector<Jugada *> Partida::getJugadas()
{
    return jugadas;
}

bool Partida::getOrientation()
{
    return orientation;
}

void Partida::setOrientation(bool orientation)
{
    this->orientation = orientation;
}

void Partida::shortCastle() {
    shortCastling = true;
}

void Partida::longCastle() {
    longCastling = true;
}

bool Partida::getShortCastling() {
    return shortCastling;
}

bool Partida::getLongCastling() {
    return longCastling;
}