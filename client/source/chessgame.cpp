#include "chessgame.hpp"
#include "configuration.cpp"

ChessGame::ChessGame(User *whitePlayer, User *blackPlayer, sf::Font *font)
{
    this->whitePlayer = whitePlayer;
    this->blackPlayer = blackPlayer;
    this->fecha = time(0);

    this->gameInfo = new GameInfo(whitePlayer, blackPlayer, &moves, font);

    // selected = false;
    selectedPiece = nullptr;
    potentiallyPieceEnPassant = nullptr;
    turn = true;

    orientation = true;
    whiteCanShortCastling = true;
    whiteCanLongCastling = true;
    blackCanShortCastling = true;
    blackCanLongCastling = true;

    finished = false;

    // tablero vacio
    for (int i = 0; i < 64; i++)
    {
        tablero[i] = nullptr;
    }

    loadFen();
    load();
}

void ChessGame::loadFen(std::string fen)
{
    //"rnbqkbnr/ pppppppp/     8     /    8    /     8    /     8    /PPPPPPPP/RNBQKBNR"
    /* 56 - 63 / 48 - 55 / *40 - 47 / *32 - 39 / *24 - 31 / *16 - 23 / 8 - 15 / 0 - 7 */
    int i = 0;
    for (int pos = 56; i < int(fen.length()); pos++)
    {

        char aux = fen.at(i);
        if (aux >= 'A' && aux <= 'Z')
        {
            whitePieces.push_back((tablero[pos] = Piece::create(pos, aux)));
        }
        else if (aux >= 'a' && aux <= 'z')
        {
            blackPieces.push_back((tablero[pos] = Piece::create(pos, aux)));
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

std::string ChessGame::saveFen()
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

void ChessGame::load(sf::Color col1, sf::Color col2)
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

void ChessGame::draw(sf::RenderTarget &target, sf::RenderStates states) const
{

    target.clear(sf::Color(128, 128, 128, 0));

    for (int i = 0; i < 64; i++)
    {
        target.draw(m_boardSquares[i]);
    }

    for (auto const &i : whitePieces)
    {
        target.draw(*i);
    }
    for (auto const &i : blackPieces)
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

std::vector<int> ChessGame::filterValidMovements(Piece *p)
{

    int pos = p->getPos();
    std::vector<int> movements = p->calcularMovimiento();
    
    // std::cout << "movements: ";
    // for (auto m : movements)
    // {
    //     std::cout << m << " ";
    // }

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

            if (isSameColor) // borramos, (no se vale ese movimiento porque no puedes comer una pieza de tu color)
            {
                // std::cout << "borrando movimeinto a " << *it << " (mismo color)" << std::endl;
                movements.erase(it);
            }
            else
            { // avanzamos, se puede comer, por lo que no se elimina el movimiento
                if (instanceof <Pawn>(p))
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

            int deleted = 0;

            int i = 1;
            while (it != end(movements) && i < 7) // i < 7
            {
                // if ((*(it) % diff) == (pos))
                // std::cout << "fs: " << firstSquare << ", *it: " << *it << ", i: " << i << std::endl;
                if (firstSquare == (*it - i * diff) && abs(*it - pos) <= abs(diff * 7))
                {
                    // std::cout << "borrando! " << *(it) << std::endl;
                    movements.erase(it);
                    // size--;
                    deleted++;
                    // i--;
                }
                else
                {
                    //std::cout << *(it) << " sale de la secuencia actual" << std::endl;
                    break;
                }
                i++;
            }
            //}
        }
        else // no piece!!!!!!
        {
            if (instanceof <Pawn>(p)) // check en passant (must be pawn)!
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
                else if (abs(*it - pos) == 16)
                {
                    if (turn)
                    {
                        // row is ok
                        if ((pos / 8 != 1) || tablero[pos + 8] != nullptr)
                        {
                            movements.erase(it);
                        }
                        else
                        {
                            *it++;
                        }
                    }
                    else
                    {
                        if ((pos / 8 != 6) || tablero[pos - 8] != nullptr)
                        {
                            movements.erase(it);
                        }
                        else
                        {
                            *it++;
                        }
                    }
                }
                else
                {
                    *it++;
                }
            }
            else if (instanceof <King>(p))
            {
                Move *lastMove = nullptr;
                if (!moves.empty())
                {
                    lastMove = moves.back();
                }
                if ((*it - pos) == 2 && p->getTimesMoved() == 0) // enroque corto
                {
                    Piece *rook = tablero[pos + 3];

                    if (rook != nullptr && instanceof <Rook>(rook) && p->getTimesMoved() == 0 && rook->getTimesMoved() == 0 && rook->getColor() == p->getColor() && lastMove != nullptr && !lastMove->isCheck() && tablero[pos + 1] == nullptr)
                    {
                        it++;
                        if (p->getColor())
                        {
                            whiteCanShortCastling = true;
                        }
                        else
                        {
                            blackCanShortCastling = true;
                        }
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
                else if ((*it - pos) == -2 && p->getTimesMoved() == 0) // enroque largo
                {
                    Piece *rook = tablero[pos - 4];
                    if (rook != nullptr && instanceof <Rook>(rook) && p->getTimesMoved() == 0 && rook->getTimesMoved() == 0 && rook->getColor() == p->getColor() && lastMove != nullptr && !lastMove->isCheck() && tablero[pos - 1] == nullptr)
                    {
                        it++;
                        if (p->getColor())
                        {
                            whiteCanLongCastling = true;
                        }
                        else
                        {
                            blackCanLongCastling = true;
                        }
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

    // std::cout << "new movements << (" << p->getNameFEN() << "): ";
    // for (auto m : movements)
    // {
    //     std::cout << toChessPosition(m) << " ";
    // }
    // std::cout << std::endl;

    return movements;
}

/*
Return true if there is any check in the actual position.
*/
bool ChessGame::checkIfChecks(bool color)
{
    // if white pieces
    if (color)
    {
        // we iterate black pieces to see if they are checking white king
        for (auto const &i : blackPieces)
        {
            if (isChecking(i))
            {
                // j->setJaque(true);
                // j->checkingPieces.push_back(i);
                return true;
            }
        }
    }
    else
    {
        for (auto const &i : whitePieces)
        {
            if (isChecking(i))
            {
                // j->setJaque(true);
                // j->checkingPieces.push_back(i);
                return true;
            }
        }
    }
    return false;
}

bool ChessGame::canPieceMove(Piece *p)
{
    std::vector<int> movements = filterValidMovements(p);
    // check movements for the other sides
    movements = filterIllegalMoves(p, movements, !turn);
    return !movements.empty();
}

/*
Check if 'color' side is being checkmated
*/
bool ChessGame::checkIfCheckmate(bool color)
{

    if (color)
    {
        for (auto const &i : blackPieces)
        {
            bool canMove = canPieceMove(i);
            if (canMove)
            {
                std::cout << "Turn " << turn << " Piece " << i->getNameFEN() << " (" << toChessPosition(i->getPos()) << ") can move!" << std::endl;
                return false;
            }
        }
    }
    else
    {
        for (auto const &i : whitePieces)
        {
            bool canMove = canPieceMove(i);
            if (canMove)
            {
                std::cout << "Turn " << turn << " Piece " << i->getNameFEN() << " (" << toChessPosition(i->getPos()) << ") can move!" << std::endl;
                return false;
            }
        }
    }
    return true;
}

/*
Return true if a given Piece p is checking the opossite King, false if not.
*/
bool ChessGame::isChecking(Piece *p)
{
    bool check = false;
    // once moved, we check new movements
    std::vector<int> movements = filterValidMovements(p);
    for (auto it = begin(movements); it != end(movements); *it++)
    {
        Piece *checkedPiece = tablero[*it];
        // if (checkedPiece != nullptr && instanceof <King>(checkedPiece))
        // {
        //     std::cout << p->getNameFEN() << "(" << p->getPos() << ") is checking " << checkedPiece->getNameFEN() << "(" << checkedPiece->getPos() << ")" << std::endl;
        // }
        if (checkedPiece != nullptr && checkedPiece->getColor() != p->getColor() && instanceof <King>(checkedPiece))
        {
            check = true;
        }
    }
    return check;
}

void ChessGame::undoPlay(int nPlay)
{
    Move *j;

    // takeback!
    if (nPlay == -1)
    {
        j = moves.back();
        moves.pop_back();
        turn = !turn;
    }
    else // go back  
    {
        j = moves.at(nPlay);
    }
    Piece *piece = j->getPiece();
    Piece *eatenPiece = j->getEatenPiece();

    // std::cout << "pieza: " << piece->getNameFEN() << " -> (" << toChessPosition(j->getPrevPos()) << ", " << toChessPosition(j->getNewPos()) << ") [" << piece->getTimesMoved() << "]" << std::endl;

    // se ha comido una pieza
    if (eatenPiece != nullptr)
    {
        if (eatenPiece->getColor())
        {
            for (auto it = begin(whitePiecesKilled); it != end(whitePiecesKilled); it++)
            {
                if (*it == eatenPiece)
                {
                    whitePiecesKilled.erase(it);
                    break;
                }
            }
            whitePieces.push_back(eatenPiece);
        }
        else
        {
            for (auto it = begin(blackPiecesKilled); it != end(blackPiecesKilled); it++)
            {
                if (*it == eatenPiece)
                {
                    blackPiecesKilled.erase(it);
                    break;
                }
            }
            blackPieces.push_back(eatenPiece);
        }
        eatenPiece->move(j->getNewPos(), false); // not really move, is undo/takeback
        tablero[j->getNewPos()] = eatenPiece;

        piece->move(j->getPrevPos(), false); // not really move, is undo
        // if (j->isFirstPieceMove()){
        //     j->setFirstPieceMoved(false);
        // }
        tablero[j->getPrevPos()] = piece;
    }
    else
    { // no se come pieza, solo se mueve
        tablero[j->getNewPos()] = nullptr;
        piece->move(j->getPrevPos(), false); // not really move, is undo/takeback
        tablero[j->getPrevPos()] = piece;
    }

    // NO HACEMOS AL PASO (MUCHO FOLLON CREO)
    // ENROQUE TAMPOCO
}

bool ChessGame::applyPlay(int nPlay)
{
    Move *j = moves.at(nPlay);
    Piece *piece = j->getPiece();

    std::cout << "pieza: " << piece->getNameFEN() << " -> (" << toChessPosition(j->getPrevPos()) << ", " << toChessPosition(j->getNewPos()) << ") [" << piece->getTimesMoved() << "]" << std::endl;

    std::vector<int> validMovements = filterValidMovements(piece);

    std::cout << "movements: ";
    for (auto m : validMovements)
    {
        std::cout << m << " ";
    }
    std::cout << std::endl;

    return applyMove(j, validMovements);
}

bool ChessGame::applyMove(Move *j, std::vector<int> movements)
{

    // se mira si se puede hacer (la Piezq tiene movimeinto, no hay jaque)

    // si se puede hacer, se mueve, se actualiza el tablero y se devuelve true (y se mete a la lista de moves)

    bool is_aplicable = false;

    Piece *pieza = j->getPiece();
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
        //selectedPiece = nullptr;
        is_aplicable = true;

        if (instanceof <King>(pieza))
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

        // quitamos movimiento del antiguo
        tablero[pieza->getPos()] = nullptr;

        // si ya habia una pieza, se come y se elimina
        int enemyPos;
        if (enPassant)
        {
            enemyPos = newpos - 8;
        }
        else
        {
            enemyPos = newpos;
        }
        Piece *pieza_enemiga = tablero[enemyPos];
        // comer
        if (pieza_enemiga != nullptr && pieza_enemiga->getColor() != pieza->getColor())
        {
            if (!turn) // blanca
            {
                for (auto it = begin(whitePieces); it != end(whitePieces);)
                {
                    if ((*it)->getPos() == enemyPos)
                    {
                        whitePieces.erase(it);
                        break;
                    }
                    else
                    {
                        it++;
                    }
                }
            }
            else
            {
                for (auto it = begin(blackPieces); it != end(blackPieces);)
                {
                    if ((*it)->getPos() == enemyPos)
                    {
                        blackPieces.erase(it);
                        break;
                    }
                    else
                    {
                        it++;
                    }
                }
            }

            // not deleted, it goest to whitePieceKilled!
            //delete tablero[enemyPos];
            
            
            if (pieza_enemiga->getColor())
            {
                whitePiecesKilled.push_back(pieza_enemiga);
            }
            else
            {
                blackPiecesKilled.push_back(pieza_enemiga);
            }
            j->eat(pieza_enemiga);
        }

        if (possibleShortCastling)
        {
            Piece *rook = tablero[pieza->getPos() + 3];
            rook->move(pieza->getPos() + 1);
            tablero[pieza->getPos() + 3] = nullptr;
            tablero[pieza->getPos() + 1] = rook;
            j->shortCastle(); // in the play
            // shortCastle();    // in the game itself
            //  0-0
        }
        if (possibleLongCastling)
        {

            Piece *rook = tablero[pieza->getPos() - 4];
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

        // DISABLED
        // if (turn)
        // {
        //     for (auto const &i : blackPieces)
        //     {
        //         if (! instanceof <King>(i) && isChecking(i))
        //         {
        //             j->setJaque(true);
        //             j->checkingPieces.push_back(i);
        //         }
        //     }
        // }
        // else
        // {
        //     for (auto const &i : whitePieces)
        //     {
        //         if (! instanceof <King>(i) && isChecking(i))
        //         {
        //             j->setJaque(true);
        //             j->checkingPieces.push_back(i);
        //         }
        //     }
        // }

        // std::cout << "total checks: " << j->checkingPieces.size() << std::endl;
    }

    // si no se puede hacer se devuelve false y no se le pasa el turno al otro jugador

    potentiallyPieceEnPassant = nullptr;

    return is_aplicable;
}

std::vector<int> ChessGame::selectPiece(int pos)
{

    //std::vector<int> validMovements;
    selectedPiece = nullptr;

    if (turn)
    {
        for (auto const &i : whitePieces)
        {
            if (i->getPos() == pos)
            {
                selectedPiece = i;
                break;
            }
        }
    }
    else
    {
        for (auto const &i : blackPieces)
        {
            if (i->getPos() == pos)
            {
                selectedPiece = i;
                break;
            }
        }
    }

    if (selectedPiece == nullptr)
    {
        possibleMovesSquares.clear();
        return validMovements;
    }
    else // piece !
    {

        potentiallyPieceEnPassant = nullptr;

        if (instanceof <Pawn>(selectedPiece) && moves.size() > 0)
        {

            Move *previousPlay = moves.back();
            Piece *previousPiece = previousPlay->getPiece();

            if (instanceof <Pawn>(previousPiece) && previousPiece->getColor() != selectedPiece->getColor() && previousPlay->isFirstPieceMove())
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

std::vector<int> ChessGame::filterIllegalMoves(Piece *p, std::vector<int> filteredMovements, bool color)
{
    // check if after all, there is a check
    // aux piece for checking!
    // std::cout << "FEN ANTES: " << saveFen() << std::endl;

    // check king castle!
    std::vector<int> erasedMovs;

    for (auto mov = begin(filteredMovements); mov != end(filteredMovements);)
    {
        Piece *enemyPiece = tablero[*mov];
        Piece *aux = Piece::create(p->getPos(), p->getNameFEN());

        // pos of the piece in list of pieces per movement
        std::vector<Piece *>::iterator itPos;
        std::vector<Piece *>::iterator itPos2;
        // Piece *aux2 = nullptr;
        aux->setPos(*mov);
        // color = true it's white's turn
        if (color)
        {
            // we delete Piece p and introduce the new one
            for (auto i = begin(whitePieces); i != end(whitePieces);)
            {
                if (*i == p)
                {
                    itPos = whitePieces.erase(i);
                    whitePieces.insert(itPos, aux);
                    break;
                }
                else
                {
                    i++;
                }
            }
            // if there is some enemy piece to eat, must be cleaned now to make calculus
            for (auto i = begin(blackPieces); i != end(blackPieces);)
            {
                if ((*i)->getPos() == (*mov))
                {
                    itPos2 = blackPieces.erase(i);
                }
                else
                {
                    i++;
                }
            }
        }
        else
        {
            for (auto i = begin(blackPieces); i != end(blackPieces);)
            {
                if (*i == p)
                {
                    itPos = blackPieces.erase(i);
                    blackPieces.insert(itPos, aux);
                    break;
                }
                else
                {
                    i++;
                }
            }
            for (auto i = begin(whitePieces); i != end(whitePieces);)
            {
                if ((*i)->getPos() == (*mov))
                {
                    itPos2 = whitePieces.erase(i);
                    // aux2 = *i;
                    break;
                }
                else
                {
                    i++;
                }
            }
        }

        // cambiamos el tablero momentaneamente
        int currentSquare = *mov;
        // std::cout << "piezas negras: ";
        // for (auto const &i : blackPieces){
        //     std::cout << i->getNameFEN() << "(" << toChessPosition(i->getPos()) << ") ";
        // }
        // std::cout << std::endl;
        // std::cout << "Ponemos la casilla " << toChessPosition(p->getPos()) << " a null" << std::endl;
        tablero[p->getPos()] = nullptr;
        tablero[currentSquare] = aux;
        // std::cout << "Ponemos en la casilla " << toChessPosition(*it) << " la pieza aux" << std::endl;

        // std::cout << "CHECKING CHECKS PER ENEMY PIECE! (FOR MOVE " << toChessPosition(*it) << ")" << std::endl;
        // if check , cannot move (erase movement) :)
        if (checkIfChecks(color))
        {
            erasedMovs.push_back(*mov);
            filteredMovements.erase(mov);
        }
        else // can move!
        {
            if (instanceof <King>(p) && p->getTimesMoved() == 0)
            {
                if (std::count(erasedMovs.begin(), erasedMovs.end(), p->getPos() + 1) && (*mov == p->getPos() + 2))
                {
                    filteredMovements.erase(mov);
                }
                else if (std::count(erasedMovs.begin(), erasedMovs.end(), p->getPos() - 1) && (*mov == p->getPos() - 2))
                {
                    filteredMovements.erase(mov);
                }
                else
                {
                    mov++;
                }
            }
            else
            {
                mov++;
            }
        }
        // std::cout << "STOPPING CHECKING CHECKS" << std::endl;

        // std::cout << "piezas blancas: ";
        // for (auto const &i : whitePieces){
        //     std::cout << i->getNameFEN() << "(" << toChessPosition(i->getPos()) << ") ";
        // }
        // std::cout << std::endl;
        // std::cout << "FEN: " << saveFen() << std::endl;

        if (color) // blancas
        {
            whitePieces.erase(itPos);
            whitePieces.insert(itPos, p);
            // whitePieces.pop_back();
            // whitePieces.push_back(p);
            if (enemyPiece != nullptr)
            {
                blackPieces.insert(itPos2, enemyPiece);
            }
        }
        else
        {
            blackPieces.erase(itPos);
            blackPieces.insert(itPos, p);
            // blackPieces.pop_back();
            // blackPieces.push_back(p);
            if (enemyPiece != nullptr)
            {
                whitePieces.insert(itPos2, enemyPiece);
            }
        }

        // ponemos el tablero como antes
        // std::cout << "Ponemos la casilla " << toChessPosition(p->getPos()) << " a null" << std::endl;n
        if (enemyPiece != nullptr && enemyPiece->getColor() != p->getColor())
        {
            tablero[currentSquare] = enemyPiece;
        }
        else
        {
            tablero[currentSquare] = nullptr;
        }
        tablero[p->getPos()] = p;

        // delete aux piece
        delete aux;

        // std::cout << "piezas blancas: ";
        // for (auto const &i : whitePieces)
        // {
        //     std::cout << i->getNameFEN() << "(" << toChessPosition(i->getPos()) << ") ";
        // }
        // std::cout << std::endl;
        // std::cout << "FEN: " << saveFen() << std::endl;
    }
    // std::cout << "STOPPING ALL VERIFICATIONS" << std::endl;

    return filteredMovements;
}

std::vector<int> ChessGame::createMovesSquares()
{
    // valid movements are those which cannot jump pieces or eat same color, as well as en passant and castling.
    std::vector<int> validMovements = filterValidMovements(selectedPiece);
    // illegal moves are those which moving would end up with a check (pinned piece, moving king to incorrect position or castling bad).
    // check movements for the same side
    validMovements = filterIllegalMoves(selectedPiece, validMovements, turn);

    // std::cout << "new movements!: ";
    // for (auto m : validMovements)
    // {
    //     std::cout << m << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "FEN!: " << saveFen() << std::endl;

    if (selectedPiece == nullptr)
        return validMovements;

    possibleMovesSquares.clear();

    for (int i = 0; i < (int)validMovements.size(); i++)
    {
        sf::RectangleShape tmp;
        // if (orientation){
        //     tmp.setPosition(sf::Vector2f((validMovements.at(i) % 8) * OBJECT_SIZE_F, (7 - (validMovements.at(i) / 8)) * OBJECT_SIZE_F));
        // }
        // else {
        //     tmp.setPosition(sf::Vector2f((7 - (validMovements.at(i) % 8)) * OBJECT_SIZE_F, (validMovements.at(i) / 8) * OBJECT_SIZE_F));
        // }
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

Move* ChessGame::moveSelected(int pos)
{

    // no piece or not selected or piece but same square as before
    if (selectedPiece == nullptr || (selectedPiece != nullptr && selectedPiece->getPos() == pos))
    {
        // std::cout << "same square, deselecting current piece" << std::endl;
        selectedPiece = nullptr;
        validMovements.clear();
        return nullptr;
    }
    // another piece
    else if (selectedPiece != nullptr && tablero[pos] != nullptr && tablero[pos]->getColor() == selectedPiece->getColor())
    {
        // std::cout << "selecting another piece" << std::endl;
        validMovements.clear();
        selectPiece(pos);
        return nullptr;
    }
    // no possible movements
    else if (validMovements.empty())
    {
        // std::cout << "there is no movements" << std::endl;
        return nullptr;
    }

    // if everything ok, new play is created
    Move *j = new Move(selectedPiece, pos);

    bool pieceHasMoved = (selectedPiece->getTimesMoved() > 0);

    bool valid = applyMove(j, validMovements);

    if (valid)
    {
        moves.push_back(j);

        // si no ha movido antes ponemos qeue esta jugada es la primera en la que mueve
        if (!pieceHasMoved)
        {
            j->setFirstPieceMoved(true); // ponemos que ha sido el primer movimiento de la pieza
        }
        // ha movido
        else
        {
        }

        if (instanceof <Pawn>(selectedPiece))
        {
            if (selectedPiece->getColor())
            {
                if (selectedPiece->getPos() / 8 == 7)
                {
                    Piece *promoted = promote(selectedPiece, DEFAULT_PROMOTION);
                    j->setPiece(promoted);
                    j->setPromoted(true);
                    j->setPawnBeforePromoting(selectedPiece);
                }
            }
            else
            {
                if (selectedPiece->getPos() / 8 == 0)
                {
                    Piece *promoted = promote(selectedPiece, DEFAULT_PROMOTION);
                    j->setPiece(promoted);
                    j->setPromoted(true);
                    j->setPawnBeforePromoting(selectedPiece);
                }
            }
        }

        bool isDraws = checkIfDrawsByMaterial();
        if (isDraws)
        {
            finished = true;
            setResult(Result::DRAW);
        }

        // check if check or checkmate
        bool isCheck = checkIfChecks(!turn);
        if (isCheck)
        {
            j->setCheck(isCheck);
            bool isCheckmate = checkIfCheckmate(turn);
            j->setCheckmate(isCheckmate);
            if (isCheckmate)
            {
                finished = true;
                if (turn)
                {
                    setResult(Result::WHITE);
                }
                else
                {
                    setResult(Result::BLACK);
                }
            }
        }
        else
        {
            // MIRAR
            bool isDraws = checkIfDrawsByPosition(turn);
            if (isDraws)
            {
                finished = true;
                setResult(Result::DRAW);
            }
        }

        // update play info
        j->generateString();
        // std::cout << "Move " << moves.size() << " (" << j->getPiece()->getColor() << "): " << j->to_string() << std::endl;
        gameInfo->updateMove();

        // deselecting current piece
        selectedPiece = nullptr;
        // turn changing
        turn = !turn;
        return j;
    }
    else {
        return nullptr;
    }
}

Piece *ChessGame::promote(Piece *p, char pieceNameNotation)
{
    Piece *newPiece = nullptr;
    std::vector<Piece *>::iterator itPos;
    if (p->getColor())
    {
        for (auto it = begin(whitePieces); it != end(whitePieces);)
        {
            if (*it == p)
            {
                newPiece = Piece::create(p->getPos(), pieceNameNotation);
                tablero[p->getPos()] = newPiece;
                itPos = whitePieces.erase(it);
                whitePieces.insert(itPos, newPiece);
                break;
            }
            else
            {
                *it++;
            }
        }
    }
    else
    {
        for (auto it = begin(blackPieces); it != end(blackPieces);)
        {
            if (*it == p)
            {
                newPiece = Piece::create(p->getPos(), pieceNameNotation + 32);
                tablero[p->getPos()] = newPiece;
                itPos = blackPieces.erase(it);
                blackPieces.insert(itPos, newPiece);
                break;
            }
            else
            {
                it++;
            }
        }
    }
    // it should not be deleted, must be stored at whiteKilledPieces...
    if (p->getColor()){
        whitePiecesKilled.push_back(p);
    }
    else{
        blackPiecesKilled.push_back(p);
    }
    //delete p;

    std::cout << "newPiece: " << newPiece->getNameFEN() << " " << toChessPosition(newPiece->getPos()) << std::endl;

    return newPiece;
}

bool ChessGame::checkIfDrawsByPosition(bool color)
{
    bool draws = true;
    if (color)
    {
        for (auto const &p : blackPieces)
        {
            bool canMove = canPieceMove(p);
            if (canMove)
            {
                draws = false;
                break;
            }
        }
    }
    else
    {
        for (auto const &p : whitePieces)
        {
            bool canMove = canPieceMove(p);
            if (canMove)
            {
                draws = false;
                break;
            }
        }
    }
    return draws;
}

bool ChessGame::checkIfDrawsByMaterial()
{
    bool draws = true;
    if (whitePieces.size() <= 2)
    {
        for (auto const &p : whitePieces)
        {
            if (instanceof <Rook>(p) || instanceof <Queen>(p) || instanceof <Pawn>(p))
            {
                draws = false;
                break;
            }
        }
    }
    else
    {
        draws = false;
    }

    if (blackPieces.size() <= 2)
    {
        for (auto const &p : blackPieces)
        {
            if (instanceof <Rook>(p) || instanceof <Queen>(p) || instanceof <Pawn>(p))
            {
                draws = false;
                break;
            }
        }
    }
    else
    {
        draws = false;
    }

    return draws;
}

bool ChessGame::rotateBoard()
{
    orientation = !orientation;
    for (auto const &i : whitePieces)
    {
        i->rotate(orientation);
    }
    for (auto const &i : blackPieces)
    {
        i->rotate(orientation);
    }

    if (selectedPiece)
    {
        // std::cout << "selecting" << 63 - selectedPiece->getPos() << std::endl;
        selectPiece(63 - selectedPiece->getPos());
    }

    return orientation;
}

std::string ChessGame::savePgn()
{
    std::string pgn;

    time_t now = time(0);
    tm *ltm = localtime(&now);

    std::string date = std::to_string(1900 + ltm->tm_year) + '.' + std::to_string(1 + ltm->tm_mon) + '.' + std::to_string(ltm->tm_mday);

    // write metadata
    pgn += "[Event \"C-hess Game\"]\n";
    pgn += "[Site \"C-hess Platform\"]\n";
    pgn += "[Date \"" + date + "\"]\n";
    pgn += "[Round \"1\"]\n";
    pgn += "[White \"" + whitePlayer->getUsername() + "\"]\n";
    pgn += "[Black \"" + blackPlayer->getUsername() + "\"]\n";
    pgn += "[WhiteElo \"" + std::to_string(whitePlayer->getElo()) + "\"]\n";
    pgn += "[BlackElo \"" + std::to_string(blackPlayer->getElo()) + "\"]\n";

    std::string result;
    if (finished)
    {
        switch (r)
        {
        case Result::WHITE:
            result += "1-0";
            break;
        case Result::BLACK:
            result += "0-1";
            break;
        case Result::DRAW:
            result += "1/2-1/2";
        default:
            break;
        }
        pgn += "[Result \"" + result + "\"]\n";
    }
    pgn += '\n';

    // write plays
    int cont = 1;
    for (auto const &i : moves)
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
    if (finished)
    {
        pgn += result;
    }

    return pgn;
}

void ChessGame::setResult(Result r)
{
    this->r = r;
    // user's calculations
    whitePlayer->calculateNewElo(blackPlayer->getElo(), float(r/2));
    blackPlayer->calculateNewElo(whitePlayer->getElo(), float((2-r)/2));
    gameInfo->updateElo(whitePlayer->getElo(), blackPlayer->getElo());
}

void ChessGame::deselectPiece(){
    selectedPiece = nullptr;
    validMovements.clear();
}

Piece *ChessGame::getPieceByPos(std::string pos)
{
    return tablero[fromChessPosition(pos)];
}

std::vector<Move *>* ChessGame::getMoves()
{
    return &moves;
}

bool ChessGame::getOrientation()
{
    return orientation;
}

void ChessGame::setOrientation(bool orientation)
{
    this->orientation = orientation;
}

Piece *ChessGame::getSelectedPiece()
{
    return selectedPiece;
}

bool ChessGame::isFinished()
{
    return finished;
}

std::vector<Piece*>* ChessGame::getWhitePieces()
{
    return &whitePieces;
}

std::vector<Piece*>* ChessGame::getBlackPieces()
{
    return &blackPieces;
}

void ChessGame::passTurn()
{
    turn = !turn;
}