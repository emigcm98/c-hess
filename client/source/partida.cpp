#include "partida.hpp"
#include "configuration.cpp"

Partida::Partida(User *usuario_blancas, User *usuario_negras, sf::Font *font)
{
    this->usuario_blancas = usuario_blancas;
    this->usuario_negras = usuario_negras;
    this->fecha = time(0);

    this->gameInfo = new GameInfo(usuario_blancas, usuario_negras, &jugadas, font);

    // selected = false;
    selectedPiece = nullptr;
    potentiallyPieceEnPassant = nullptr;
    turn = true;

    orientation = true;
    whiteCanShortCastling = true;
    whiteCanLongCastling = true;
    blackCanShortCastling = true;
    blackCanLongCastling = true;

    // tablero vacio
    for (int i = 0; i < 64; i++)
    {
        tablero[i] = nullptr;
    }
    
    // loadFen("1nbqkbnr/1p1p2pp/p1P1p3/r2Bp3/2p1P3/8/PPPP2PP/RNBQK1NR b");
    // loadFen("1nbqkbnr/8/8/8/8/8/8/8 b");
    loadFen();
    load();

    std::cout << "FEN: " << saveFen() << std::endl;
}

void Partida::loadFen(std::string fen)
{
    //"rnbqkbnr/ pppppppp/     8     /    8    /     8    /     8    /PPPPPPPP/RNBQKBNR"
    /* 56 - 63 / 48 - 55 / *40 - 47 / *32 - 39 / *24 - 31 / *16 - 23 / 8 - 15 / 0 - 7 */
    int i = 0;
    for (int pos = 56; i < int(fen.length()); pos++)
    {

        char aux = fen.at(i);
        if (aux >= 'A' && aux <= 'Z')
        {
            piezas_blanco.push_back((tablero[pos] = Pieza::create(pos, aux)));
        }
        else if (aux >= 'a' && aux <= 'z')
        {
            piezas_negro.push_back((tablero[pos] = Pieza::create(pos, aux)));
        }
        else if (aux >= '1' && aux <= '8')
        {
            pos += (aux - '1');
        }
        else if (aux == '/')
        {
            pos -= 17;
        }
        else if (aux == ' ')
        {
            char next = fen.at(i + 1);
            if (next == 'w')
            {
                turn = true;
            }
            else if (next == 'b')
            {
                turn = false;
            }
            i++;
        }
        i++;
    }
}

std::string Partida::saveFen()
{
    //"rnbqkbnr/ pppppppp/     8     /    8    /     8    /     8    /PPPPPPPP/RNBQKBNR"
    /* 56 - 63 / 48 - 55 / *40 - 47 / *32 - 39 / *24 - 31 / *16 - 23 / 8 - 15 / 0 - 7 */
    std::string fen;
    int blank = 0;
    int iter = 0;
    for (int pos = 56;; pos++)
    {
        if (iter == 8)
        {
            if (blank > 0)
            {
                fen += ('0' + blank);
                blank = 0;
            }
            iter = 0;
            pos -= 16;
            if (pos < 0)
            {
                break;
            }
            fen += '/';
        }
        if (tablero[pos] != nullptr)
        {
            if (blank > 0)
            {
                fen += ('0' + blank);
            }
            fen += tablero[pos]->getNameFEN();

            blank = 0;
        }
        else
        {
            blank++;
        }
        iter++;
    }
    fen += ' ';
    fen += (turn ? 'w' : 'b');

    return fen;
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

    for (auto const &i : piezas_blanco)
    {
        target.draw(*i);
    }
    for (auto const &i : piezas_negro)
    {
        target.draw(*i);
    }

    if (selectedPiece != nullptr)
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

    // return movements; // PRUEBA

    std::cout << "movements: ";
    for (auto m : movements)
    {
        std::cout << m << " ";
    }
    std::cout << std::endl;

    // int size = (int)movements.size();

    for (auto it = begin(movements); it != end(movements);)
    {
        // piece !
        if (tablero[*it] != nullptr)
        {
            bool isSameColor = tablero[*it]->getColor() == p->getColor();
            int div;
            if (abs(*it / 8 - pos / 8) == 0)
            {
                div = abs(*it % 8 - pos % 8);
            }
            else
            {
                div = abs(*it / 8 - pos / 8);
            }
            // abs o no abs???
            int diff = (*it - pos) / div; // cantidad que se desplaza hacia esa dirección concreta OK

            int firstSquare = *it;
            std::cout << "hay una pieza en la pos " << firstSquare << std::endl;

            // if (tablero[*it]->getColor() == p->getColor()){
            //     movements.erase(it);
            // }
            if (isSameColor) // borramos, (no se vale ese movimiento porque no puedes comer una pieza de tu color)
            {
                std::cout << "borrando movimeinto a " << *it << " (mismo color)" << std::endl;
                movements.erase(it);
            }
            else
            { // avanzamos, se puede comer, por lo que no se elimina el movimiento
                if (instanceof <Peon>(p))
                {
                    // in front of or double movement, cannot eat
                    if (abs(*it - pos) == 8 || abs(*it - pos) == 16)
                    {
                        movements.erase(it);
                        continue;
                    }
                }
                *it++;
            }

            // size--;
            std::cout << "diff: " << diff << std::endl;

            int deleted = 0;

            // for (int i = 0; i < (size-1); i++) {

            // if (! instanceof <Rey>(p) && ! instanceof<Caballo>(p)) // no se si sirve
            //{
            int i = 1;
            while (it != end(movements) && i < 7)
            {
                // if ((*(it) % diff) == (pos))
                std::cout << "fs: " << firstSquare << ", *it: " << *it << ", i: " << i << std::endl;
                if (firstSquare == (*it - i * diff) && abs(*it - pos) <= abs(diff * 7))
                {
                    std::cout << "borrando! " << *(it) << std::endl;
                    movements.erase(it);
                    // size--;
                    deleted++;
                    // i--;
                }
                else
                {
                    std::cout << *(it) << " sale de la secuencia actual" << std::endl;
                    break;
                }
                i++;
            }
            //}
        }
        else // no piece!!!!!!
        {
            if (instanceof <Peon>(p)) // check en passant (must be pawn)!
            {
                // en passant
                if (abs(*it - pos) == 7)
                {
                    if (potentiallyPieceEnPassant != nullptr && tablero[pos - 1] == potentiallyPieceEnPassant)
                    {
                        it++;
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
                        it++;
                    }
                    else
                    {
                        movements.erase(it);
                    }
                }
                else
                {
                    *it++;
                }
            }
            else if (instanceof <Rey>(p))
            {
                if ((*it - p->getPos()) == 2) // enroque corto
                {
                    Pieza *rook = tablero[p->getPos() + 3];
                    if (rook != nullptr && instanceof <Torre>(rook) && !p->getMoved() && !rook->getMoved() && rook->getColor() == p->getColor())
                    {
                        it++;
                    }
                    else
                    {
                        if (p->getColor())
                        {
                            whiteCanShortCastling = false;
                        }
                        else
                        {
                            blackCanShortCastling = false;
                        }
                        movements.erase(it);
                    }
                }
                else if ((*it - p->getPos()) == -2) // enroque largo
                {
                    Pieza *rook = tablero[p->getPos() - 4];
                    if (rook != nullptr && instanceof <Torre>(rook) && !p->getMoved() && !rook->getMoved() && rook->getColor() == p->getColor())
                    {
                        it++;
                    }
                    else
                    {
                        if (p->getColor())
                        {
                            whiteCanLongCastling = false;
                        }
                        else
                        {
                            blackCanLongCastling = false;
                        }
                        movements.erase(it);
                    }
                }
                else
                {
                    *it++;
                }
            }
            else
            {
                *it++;
            }
        }
    }

    std::cout << "new movements: ";
    for (auto m : movements)
    {
        std::cout << m << " ";
    }
    std::cout << std::endl;

    return movements;
}

bool Partida::isChecking(Pieza *p)
{
    bool check = false;
    std::cout << "checking new check with piece " << p->getNameFEN() << " in pos: " << toChessPosition(p->getPos()) << std::endl;
    // once moved, we check new movements
    std::vector<int> movements = filterValidMovements(p);
    for (auto it = begin(movements); it != end(movements); *it++)
    {
        Pieza *enemyPiece = tablero[*it];
        if (enemyPiece != nullptr && enemyPiece->getColor() != p->getColor() && instanceof <Rey>(enemyPiece))
        {
            check = true;
        }
    }
    return check;
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

    // std::vector<int> movements = filterValidMovements(pieza);

    bool possibleShortCastling = false;
    bool possibleLongCastling = false;

    // newpos avalaible, movements bigger than 1
    if (pieza->getPos() == j->getNewPos()) // same square, no movement
    {
        selectedPiece = nullptr;
    }
    else if (std::count(movements.begin(), movements.end(), newpos))
    {
        selectedPiece = nullptr;
        is_aplicable = true;

        if (instanceof <Rey>(pieza))
        {
            if (newpos - pieza->getPos() == 2) // enroque corto
            {
                if (pieza->getColor() && whiteCanShortCastling)
                {
                    possibleShortCastling = true;
                }
                else if (!pieza->getColor() && blackCanShortCastling)
                {
                    possibleShortCastling = true;
                }
            }
            else if (newpos - pieza->getPos() == -2) // enroque largo
            {
                if (pieza->getColor() && whiteCanLongCastling)
                {
                    possibleLongCastling = true;
                }
                else if (!pieza->getColor() && blackCanLongCastling)
                {
                    possibleLongCastling = true;
                }
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
            j->eat();
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
            // shortCastle();    // in the game itself
            //  0-0
        }
        if (possibleLongCastling)
        {

            Pieza *rook = tablero[pieza->getPos() - 4];
            rook->move(pieza->getPos() - 1);
            tablero[pieza->getPos() - 4] = nullptr;
            tablero[pieza->getPos() - 1] = rook;
            j->longCastle(); // in the play
            // longCastle();    // in the game itself
            //  0-0-0
        }

        // movemos la pieza
        pieza->move(newpos);
        // ponemos el nuevo
        tablero[newpos] = pieza;

        // check if it is check after move

        if (turn)
        {
            for (auto const &i : piezas_blanco)
            {
                if (! instanceof <Rey>(i) && isChecking(i))
                {
                    j->setJaque(true);
                    j->checkingPieces.push_back(i);
                }
            }
        }
        else
        {
            for (auto const &i : piezas_blanco)
            {
                if (! instanceof <Rey>(i) && isChecking(i))
                {
                    j->setJaque(true);
                    j->checkingPieces.push_back(i);
                }
            }
        }

        std::cout << "total checks: " << j->checkingPieces.size() << std::endl;

        // if (isChecking(pieza)){
        //     j->setJaque(true);
        // }
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
                break;
            }
        }
        else
        {
            if (piezas_negro[i]->getPos() == pos)
            {
                selectedPiece = piezas_negro[i];
                break;
            }
        }
        selectedPiece = nullptr;
    }

    if (selectedPiece == nullptr)
    {
        possibleMovesSquares.clear();
        return validMovements;
    }
    else // piece !
    {

        potentiallyPieceEnPassant = nullptr;

        if (instanceof <Peon>(selectedPiece) && jugadas.size() > 0)
        {

            Jugada *previousPlay = jugadas.back();
            Pieza *previousPiece = previousPlay->getPieza();

            if (instanceof <Peon>(previousPiece) && previousPiece->getColor() != selectedPiece->getColor() && previousPlay->isFirstPieceMove())
            {
                if (abs(previousPiece->getPos() - selectedPiece->getPos()) == 1) // -1 or 1
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
            // blue
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

void Partida::moveSelected(int pos, std::vector<int> validMovements)
{

    // no piece or not selected or piece but same square as before
    if (selectedPiece == nullptr || (selectedPiece != nullptr && selectedPiece->getPos() == pos))
    {
        selectedPiece = nullptr;
        return;
    }
    // another piece
    else if (selectedPiece != nullptr && tablero[pos] != nullptr && tablero[pos]->getColor() == selectedPiece->getColor())
    {
        selectPiece(pos);
        return;
    }
    // no possible movements
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
        else
        { // ha movido
            if (instanceof <Rey>(selectedPiece))
            { // if king, then cannot castle anymore
                if (selectedPiece->getColor())
                { // white
                    whiteCanLongCastling = false;
                    whiteCanShortCastling = false;
                }
                else
                {
                    blackCanShortCastling = false;
                    blackCanLongCastling = false;
                }
            }
        }
        gameInfo->updateJugada();
        j->generateString();
        std::cout << j->to_string() << std::endl;
    }

    selectedPiece = nullptr;
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

    if (selectedPiece)
    {
        std::cout << "selecting" << 63 - selectedPiece->getPos() << std::endl;
        selectPiece(63 - selectedPiece->getPos());
    }
}

std::string Partida::savePgn()
{
    std::string pgn;

    // write metadata

    // write plays
    int cont = 1;
    for (auto const &i : jugadas)
    {
        if (cont % 2 == 1)
        {
            pgn += std::to_string(cont / 2 + 1);
            pgn += ". ";
        }
        pgn += (i->to_string() + ' ');
        cont++;
    }
    // write result

    return pgn;
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

std::vector<Jugada *> Partida::getJugadas()
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

// void Partida::shortCastle()
// {
//     shortCastling = true;
// }

// void Partida::longCastle()
// {
//     longCastling = true;
// }

// bool Partida::getShortCastling()
// {
//     return shortCastling;
// }

// bool Partida::getLongCastling()
// {
//     return longCastling;
// }

Pieza *Partida::getSelectedPiece()
{
    return selectedPiece;
}