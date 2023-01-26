#include "chessgame.hpp"
#include "configuration.cpp"

ChessGame::ChessGame(User *whitePlayer, User *blackPlayer, sf::Font *font)
{
    this->whitePlayer = whitePlayer;
    this->blackPlayer = blackPlayer;
    this->fecha = time(0);

    this->gameInfo = new GameInfo(whitePlayer, blackPlayer, &moves, font);
    this->promotionComponent = nullptr;

    selectedPiece = nullptr;
    potentiallyPieceEnPassant = nullptr;
    turn = true; // white start

    orientation = true;

    // was true before, but load in FEN
    whiteCanShortCastling = false;
    whiteCanLongCastling = false;
    blackCanShortCastling = false;
    blackCanLongCastling = false;

    selectingPromoted = false;

    fiftyMoveRuleNumber = 0;

    finished = false;

    // empty board
    for (int i = 0; i < 64; i++)
    {
        board[i] = nullptr;
    }

    // loadFen("rnbqkbnr/pppppppp/8/4P3/8/8/PPPP1PPP/R3K2R b");
    loadFen();
    load();
}

ChessGame::~ChessGame()
{
    // std::cout << "whitePieces:" << std::endl;
    for (auto const &i : whitePieces)
    {
        delete i;
    }
    // std::cout << "whitePiecesKilled:" << std::endl;
    for (auto const &i : whitePiecesKilled)
    {
        delete i;
    }
    // std::cout << "blackPieces:" << std::endl;
    for (auto const &i : blackPieces)
    {
        delete i;
    }
    // std::cout << "blackPiecesKilled:" << std::endl;
    for (auto const &i : blackPiecesKilled)
    {
        delete i;
    }

    // std::cout << "moves:" << std::endl;
    for (auto const &i : moves)
    {
        delete i;
    }

    // std::cout << "gameInfo:" << std::endl;
    delete gameInfo;

    if (promotionComponent != nullptr)
    {
        // std::cout << "deleting promotion component?:" << std::endl;
        delete promotionComponent;
    }
}

void ChessGame::loadFen(std::string fen)
{
    //"rnbqkbnr/ pppppppp/     8     /    8    /     8    /     8    /PPPPPPPP/RNBQKBNR"
    /* 56 - 63 / 48 - 55 / *40 - 47 / *32 - 39 / *24 - 31 / *16 - 23 / 8 - 15 / 0 - 7 */
    int i = 0;
    int blankspaces = 0;
    for (int pos = 56; i < int(fen.length()); pos++)
    {
        char aux = fen.at(i);
        if (aux >= 'A' && aux <= 'Z')
        {
            if (blankspaces == 0)
            {
                whitePieces.push_back((board[pos] = Piece::create(pos, aux)));
            }
            else if (blankspaces == 2)
            {
                if (aux == 'K')
                {
                    whiteCanShortCastling = true;
                }
                else if (aux == 'Q')
                {
                    whiteCanLongCastling = true;
                }
            }
        }
        else if (aux >= 'a' && aux <= 'z')
        {
            if (blankspaces == 0)
            {
                blackPieces.push_back((board[pos] = Piece::create(pos, aux)));
            }
            else if (blankspaces == 1)
            {
                if (aux == 'b')
                {
                    turn = false;
                }
                else if (aux == 'w')
                {
                    turn = true;
                }
            }
            else if (blankspaces == 2)
            {
                if (aux == 'k')
                {
                    blackCanShortCastling = true;
                }
                else if (aux == 'q')
                {
                    blackCanLongCastling = true;
                }
            }
        }
        else if (aux >= '0' && aux <= '8')
        {
            if (blankspaces == 0)
            {
                pos += (aux - '1');
            }
            else if (blankspaces == 4)
            {
                fiftyMoveRuleNumber = (aux - '0');
            }
        }
        else if (aux == '/')
        {
            if (blankspaces == 0)
            {
                pos -= 17;
            }
        }
        else if (aux == ' ')
        {
            blankspaces++;
        }
        // else if (aux == '-')
        // {

        // }
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
        if (board[pos] != nullptr)
        {
            if (blank > 0)
            {
                fen += ('0' + blank);
            }
            fen += board[pos]->getNameFEN();

            blank = 0;
        }
        else
        {
            blank++;
        }
        iter++;
    }
    fen += ' ';
    fen += (turn ? "w " : "b ");
    // check rook
    fen += (whiteCanShortCastling ? 'K' : '\0');
    fen += (whiteCanShortCastling ? 'Q' : '\0');
    fen += (blackCanShortCastling ? 'k' : '\0');
    fen += (blackCanShortCastling ? 'q' : '\0');
    if (!whiteCanShortCastling && !whiteCanLongCastling && !blackCanShortCastling && !blackCanLongCastling)
    {
        fen += '-';
    }
    fen += " - ";
    fen += std::to_string(fiftyMoveRuleNumber);
    fen += ' ';
    fen += std::to_string(moves.size() / 2 + 1);

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

    if (promotionComponent != nullptr)
    {
        target.draw(*promotionComponent);
    }
}

std::vector<int> ChessGame::filterValidMoves(Piece *p)
{

    int pos = p->getPos();
    std::vector<int> movements = p->calcularMovimiento();

    for (auto it = begin(movements); it != end(movements);)
    {
        // piece !
        if (board[*it] != nullptr)
        {
            bool isSameColor = board[*it]->getColor() == p->getColor();
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
                    movements.erase(it);
                    deleted++;
                }
                else
                {
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
                    if (potentiallyPieceEnPassant != nullptr && (p->getColor() ? board[pos - 1] : board[pos + 1]) == potentiallyPieceEnPassant)
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
                    if (potentiallyPieceEnPassant != nullptr && (p->getColor() ? board[pos + 1] : board[pos - 1]) == potentiallyPieceEnPassant)
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
                    // row is ok
                    if (turn ? (pos / 8 != 1) || board[pos + 8] != nullptr : (pos / 8 != 6) || board[pos - 8] != nullptr)
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
                    Piece *rook = board[pos + 3];

                    if (rook != nullptr && instanceof <Rook>(rook) && p->getTimesMoved() == 0 && rook->getTimesMoved() == 0 && rook->getColor() == p->getColor() && ((lastMove != nullptr && !lastMove->isCheck()) || moves.size() == 0) && board[pos + 1] == nullptr)
                    {
                        it++;
                        // p->getColor() ? whiteCanShortCastling = true : blackCanShortCastling = true;
                    }
                    else
                    {
                        // p->getColor() ? whiteCanShortCastling = false : blackCanShortCastling = false;
                        movements.erase(it);
                    }
                }
                else if ((*it - pos) == -2 && p->getTimesMoved() == 0) // enroque largo
                {
                    Piece *rook = board[pos - 4];
                    if (rook != nullptr && instanceof <Rook>(rook) && p->getTimesMoved() == 0 && rook->getTimesMoved() == 0 && rook->getColor() == p->getColor() && ((lastMove != nullptr && !lastMove->isCheck()) || moves.size() == 0) && board[pos - 1] == nullptr)
                    {
                        it++;
                        // p->getColor() ? whiteCanLongCastling = true : blackCanLongCastling = true;
                    }
                    else
                    {
                        // p->getColor() ? whiteCanLongCastling = false : blackCanLongCastling = false;
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
    return movements;
}

/*
Return true if there is any check in the actual position.
*/
bool ChessGame::checkIfChecks(bool color)
{

    std::vector<Piece *> *piecesList = color ? &blackPieces : &whitePieces;

    for (auto const &i : *piecesList)
    {
        if (isChecking(i))
        {
            return true;
        }
    }
    return false;
}

bool ChessGame::canPieceMove(Piece *p)
{
    std::vector<int> movements = filterValidMoves(p);
    // check movements for the other sides
    movements = filterIllegalMoves(p, movements, !turn);
    return !movements.empty();
}

/*
Check if 'color' side is being checkmated
*/
bool ChessGame::checkIfCheckmate(bool color)
{
    std::vector<Piece *> *piecesList = color ? &blackPieces : &whitePieces;

    for (auto const &i : *piecesList)
    {
        bool canMove = canPieceMove(i);
        if (canMove)
        {
            return false;
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
    std::vector<int> movements = filterValidMoves(p);
    for (auto it = begin(movements); it != end(movements); *it++)
    {
        Piece *checkedPiece = board[*it];

        if (checkedPiece != nullptr && checkedPiece->getColor() != p->getColor() && instanceof <King>(checkedPiece))
        {
            check = true;
        }
    }
    return check;
}

void ChessGame::undoMove(int nPlay)
{
    Move *j;

    // takeback!
    if (nPlay == -1)
    {
        j = moves.back();
        moves.pop_back();
        if (fiftyMoveRuleNumber > 0 || !j->getPiece()->getColor())
        {
            fiftyMoveRuleNumber--;
        }
    }
    else // go back
    {
        j = moves.at(nPlay);
    }
    Piece *piece = j->getPiece();
    Piece *eatenPiece = j->getEatenPiece();

    // std::cout << "pieza: " << piece->getNameFEN() << " -> (" << toChessPosition(j->getPrevPos()) << ", " << toChessPosition(j->getNewPos()) << ") [" << piece->getTimesMoved() << "]" << std::endl;

    // promotion
    Piece *pawn = j->getPawnBeforePromoting();
    if (pawn != nullptr)
    {
        std::cout << pawn->getNameFEN() << " " << pawn->getPos() << std::endl;
        std::vector<Piece *> *piecesList = piece->getColor() ? &whitePieces : &blackPieces;
        std::vector<Piece *>::iterator itPos;
        for (auto it = begin(*piecesList); it != end(*piecesList);)
        {
            if (*it == piece)
            {
                itPos = piecesList->erase(it);
                break;
            }
            else
            {
                it++;
            }
        }
        piecesList->insert(itPos, pawn);

        piecesList = pawn->getColor() ? &whitePiecesKilled : &blackPiecesKilled;
        for (auto it = begin(*piecesList); it != end(*piecesList); it++)
        {
            if (*it == pawn)
            {
                piecesList->erase(it);
                break;
            }
        }
        delete piece;
        piece = pawn;
    }

    board[j->getNewPos()] = nullptr;
    // se ha comido una pieza
    if (eatenPiece != nullptr)
    {
        std::vector<Piece *> *piecesKilledList = eatenPiece->getColor() ? &whitePiecesKilled : &blackPiecesKilled;
        std::vector<Piece *> *piecesList = eatenPiece->getColor() ? &whitePieces : &blackPieces;

        for (auto it = begin(*piecesKilledList); it != end(*piecesKilledList); it++)
        {
            if (*it == eatenPiece)
            {
                piecesKilledList->erase(it);
                break;
            }
        }

        piecesList->push_back(eatenPiece);

        board[eatenPiece->getPos()] = eatenPiece;
    }

    piece->move(j->getPrevPos(), false); // not really move, is undo
    board[j->getPrevPos()] = piece;

    if (j->isShortCastling())
    {
        Piece *rook = board[piece->getPos() + 1];
        if (rook != nullptr)
        {
            board[piece->getPos() + 1] = nullptr;
            rook->move(piece->getPos() + 3, false);
            board[piece->getPos() + 3] = rook;
        }
    }
    else if (j->isLongCastling())
    {
        Piece *rook = board[piece->getPos() - 1];
        if (rook != nullptr)
        {
            board[piece->getPos() - 1] = nullptr;
            rook->move(piece->getPos() - 4, false);
            board[piece->getPos() - 4] = rook;
        }
    }

    if (nPlay == -1)
    {
        turn = !turn;
    }
}

bool ChessGame::restoreMove(int nPlay)
{
    Move *j = moves.at(nPlay);
    Piece *piece = j->getPiece();

    std::cout << "pieza: " << piece->getNameFEN() << " -> (" << toChessPosition(j->getPrevPos()) << ", " << toChessPosition(j->getNewPos()) << ") [" << piece->getTimesMoved() << "]" << std::endl;
    std::cout << "pieza pos: " << toChessPosition(piece->getPos()) << std::endl;
    selectPiece(piece->getPos());
    std::cout << "movements: ";
    for (auto m : validMovements)
    {
        std::cout << m << " ";
    }
    std::cout << std::endl;

    return moveSelected(j->getNewPos(), false, true);
}

bool ChessGame::applyMove(Move *j, std::vector<int> movements)
{

    // se mira si se puede hacer (la Piezq tiene movimeinto, no hay jaque)

    // si se puede hacer, se mueve, se actualiza el board y se devuelve true (y se mete a la lista de moves)

    bool is_aplicable = false;

    Piece *pieza = j->getPiece();
    int newpos = j->getNewPos();

    if (potentiallyPieceEnPassant != nullptr)
    {
        if (abs(pieza->getPos() - newpos) == 7 || abs(pieza->getPos() - newpos) == 9)
        {
            j->setEnPassant(true);
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
        // selectedPiece = nullptr;
        is_aplicable = true;

        if (instanceof <King>(pieza))
        {
            if (newpos - pieza->getPos() == 2) // short castle
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
            else if (newpos - pieza->getPos() == -2) // long castle
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
        board[pieza->getPos()] = nullptr;

        // si ya habia una pieza, se come y se elimina
        int enemyPos = newpos;
        if (j->isEnPassant())
        {
            enemyPos = pieza->getColor() ? newpos - 8 : newpos + 8;
            std::cout << "pos en passant: " << toChessPosition(enemyPos) << std::endl;
        }

        Piece *pieza_enemiga = board[enemyPos];

        // fifty rule move number
        if (instanceof <Pawn>(pieza) || (pieza_enemiga != nullptr && pieza_enemiga->getColor() != pieza->getColor()))
        {
            fiftyMoveRuleNumber = 0;
        }
        else if (pieza->getColor())
        {
            fiftyMoveRuleNumber++;
        }

        // take piece
        if (pieza_enemiga != nullptr && pieza_enemiga->getColor() != pieza->getColor())
        {
            std::vector<Piece *> *pieceList = !turn ? &whitePieces : &blackPieces;

            for (auto it = begin(*pieceList); it != end(*pieceList);)
            {
                if ((*it)->getPos() == enemyPos)
                {
                    pieceList->erase(it);
                    break;
                }
                else
                {
                    it++;
                }
            }

            // not deleted, it goest to whitePieceKilled!
            // delete board[enemyPos];

            (pieza_enemiga->getColor() ? whitePiecesKilled : blackPiecesKilled).push_back(pieza_enemiga);
            j->eat(pieza_enemiga);
        }

        if (possibleShortCastling)
        {
            Piece *rook = board[pieza->getPos() + 3];
            rook->move(pieza->getPos() + 1);
            board[pieza->getPos() + 3] = nullptr;
            board[pieza->getPos() + 1] = rook;
            j->shortCastle(); // in the play
            // shortCastle();    // in the game itself
            //  0-0
        }
        if (possibleLongCastling)
        {

            Piece *rook = board[pieza->getPos() - 4];
            rook->move(pieza->getPos() - 1);
            board[pieza->getPos() - 4] = nullptr;
            board[pieza->getPos() - 1] = rook;
            j->longCastle(); // in the play
            // longCastle();    // in the game itself
            //  0-0-0
        }

        // movemos la pieza
        pieza->move(newpos);

        // ponemos el nuevo
        board[newpos] = pieza;
    }

    // si no se puede hacer se devuelve false y no se le pasa el turno al otro jugador

    potentiallyPieceEnPassant = nullptr;

    return is_aplicable;
}

std::vector<int> ChessGame::selectPiece(int pos)
{

    // std::vector<int> validMovements;
    selectedPiece = nullptr;

    std::vector<Piece *> *piecesList = turn ? &whitePieces : &blackPieces;

    for (auto const &i : *piecesList)
    {
        if (i->getPos() == pos)
        {
            selectedPiece = i;
            break;
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

            Move *previousMove = moves.back();
            Piece *previousPiece = previousMove->getPiece();

            if (instanceof <Pawn>(previousPiece) && previousPiece->getColor() != selectedPiece->getColor() /*&& previousMove->isFirstPieceMove()*/)
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

    // check king castle!
    std::vector<int> erasedMovs;

    for (auto mov = begin(filteredMovements); mov != end(filteredMovements);)
    {
        // Move* lastMove = nullptr;
        // if (moves.size() > 0)
        // {
        //     lastMove = moves.back();
        // }
        int enemyPos = *mov;
        // if (instanceof<Pawn>(p) && potentiallyPieceEnPassant != nullptr)
        // {
        //     enemyPos = potentiallyPieceEnPassant->getPos();
        // }
        // if (potentiallyPieceEnPassant)
        // {
        //     enemyPos = p->getColor() ? *mov-8 : *mov+8;
        // }
        Piece *enemyPiece = board[enemyPos];
        Piece *aux = Piece::create(p->getPos(), p->getNameFEN());

        // pos of the piece in list of pieces per movement
        std::vector<Piece *>::iterator itPos;
        std::vector<Piece *>::iterator itPos2;
        // Piece *aux2 = nullptr;
        aux->setPos(*mov);
        // color = true it's white's turn

        std::vector<Piece *> *piecesList = color ? &whitePieces : &blackPieces;
        std::vector<Piece *> *piecesList2 = color ? &blackPieces : &whitePieces;

        // we delete Piece p and introduce the new one
        for (auto i = begin(*piecesList); i != end(*piecesList);)
        {
            if (*i == p)
            {
                itPos = piecesList->erase(i);
                piecesList->insert(itPos, aux);
                break;
            }
            else
            {
                i++;
            }
        }
        // if there is some enemy piece to eat, must be cleaned now to make calculus
        for (auto i = begin(*piecesList2); i != end(*piecesList2);)
        {
            if ((*i)->getPos() == (*mov))
            {
                itPos2 = piecesList2->erase(i);
                break;
            }
            else
            {
                i++;
            }
        }

        // changing board status (aux)
        int currentSquare = *mov;

        board[p->getPos()] = nullptr;
        board[currentSquare] = aux;

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

        piecesList->erase(itPos);
        piecesList->insert(itPos, p);
        if (enemyPiece != nullptr)
        {
            piecesList2->insert(itPos2, enemyPiece);
        }

        // restoring board
        if (enemyPiece != nullptr && enemyPiece->getColor() != p->getColor())
        {
            board[currentSquare] = enemyPiece;
        }
        else
        {
            board[currentSquare] = nullptr;
        }
        board[p->getPos()] = p;

        // delete aux piece
        delete aux;
    }

    return filteredMovements;
}

std::vector<int> ChessGame::createMovesSquares()
{
    // valid movements are those which cannot jump pieces or eat same color, as well as en passant and castling.
    std::vector<int> validMovements = filterValidMoves(selectedPiece);
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
        if (board[validMovements.at(i)] != nullptr && board[validMovements.at(i)]->getColor() != selectedPiece->getColor()) // another piece, put in red
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

Move *ChessGame::moveSelected(int pos, bool createPromotionComponent, bool fake)
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
    else if (selectedPiece != nullptr && board[pos] != nullptr && board[pos]->getColor() == selectedPiece->getColor())
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

    // for enPassant!
    bool pieceHasMoved = (selectedPiece->getTimesMoved() > 0);

    bool valid = applyMove(j, validMovements);

    if (fake)
    {
        delete j;
        selectedPiece = nullptr;
        // if (moves.size() > 1)
        // {
            turn = !turn;
        // }
        return nullptr;
    }

    if (valid)
    {
        moves.push_back(j);

        // si no ha movido antes ponemos qeue esta jugada es la primera en la que mueve
        if (!pieceHasMoved)
        {
            j->setFirstPieceMoved(true); // ponemos que ha sido el primer movimiento de la pieza
        }
        // // ha movido
        // else
        // {
        // }

        if (instanceof <Pawn>(selectedPiece))
        {
            if (selectedPiece->getPos() / 8 == 7 || selectedPiece->getPos() / 8 == 0) // white or black last row
            {
                std::cout << "pawn arrived at " << toChessPosition(selectedPiece->getPos()) << " waiting to promote!" << std::endl;
                selectingPromoted = true;
                if (createPromotionComponent)
                {
                    promotionComponent = new PromotionComponent(pos, turn);
                    selectedPiece = nullptr;
                }
                // Piece *promoted = promote(j, DEFAULT_PROMOTION);
                return nullptr; // ??
            }
        }

        // finish, check, checkmate, draws...
        return updateStatusAfterMoving(j);
    }
    else
    {
        return nullptr;
    }
}

Move *ChessGame::updateStatusAfterMoving(Move *m)
{
    // 50*2
    bool isDraws = checkIfDrawsByMaterial() || fiftyMoveRuleNumber >= 50;
    if (isDraws)
    {
        finished = true;
        setResult(Result::DRAW);
    }

    // check if check or checkmate
    bool isCheck = checkIfChecks(!turn);
    if (isCheck)
    {
        m->setCheck(isCheck);
        bool isCheckmate = checkIfCheckmate(turn);
        m->setCheckmate(isCheckmate);
        if (isCheckmate)
        {
            finished = true;
            turn ? setResult(Result::WHITE) : setResult(Result::BLACK);

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
    m->generateString();
    // std::cout << "Move " << moves.size() << " (" << j->getPiece()->getColor() << "): " << j->to_string() << std::endl;
    gameInfo->updateMove();

    // deselecting current piece
    selectedPiece = nullptr;
    // turn changing
    turn = !turn;

    return m;
}

Piece *ChessGame::promote(Move *m, char pieceNameNotation)
{
    Piece *p = m->getPiece();
    Piece *newPiece = nullptr;
    std::vector<Piece *>::iterator itPos;

    // check color to take pieceList
    std::vector<Piece *> *piecesList = p->getColor() ? &whitePieces : &blackPieces;
    for (auto it = begin(*piecesList); it != end(*piecesList);)
    {
        if (*it == p)
        {
            newPiece = Piece::create(p->getPos(), pieceNameNotation);
            board[p->getPos()] = newPiece;
            itPos = piecesList->erase(it);
            piecesList->insert(itPos, newPiece);
            break;
        }
        else
        {
            *it++;
        }
    }

    // it should not be deleted, must be stored at whiteKilledPieces...
    p->getColor() ? whitePiecesKilled.push_back(p) : blackPiecesKilled.push_back(p);

    // delete p;

    // update the move
    m->setPiece(newPiece);
    m->setPromoted(true);
    m->setPawnBeforePromoting(p);

    updateStatusAfterMoving(m);

    selectingPromoted = false;
    if (promotionComponent != nullptr)
    {
        delete promotionComponent;
        promotionComponent = nullptr;
    }

    return newPiece;
}

bool ChessGame::checkIfDrawsByPosition(bool color)
{
    bool draws = true;
    std::vector<Piece *> *piecesList = color ? &blackPieces : &whitePieces;

    for (auto const &p : *piecesList)
    {
        bool canMove = canPieceMove(p);
        if (canMove)
        {
            draws = false;
            break;
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
    whitePlayer->calculateNewElo(blackPlayer->getElo(), (float)r / 2);
    blackPlayer->calculateNewElo(whitePlayer->getElo(), (float)((2 - (float)r) / 2));
    gameInfo->updateElo(whitePlayer->getElo(), blackPlayer->getElo());
}

void ChessGame::deselectPiece()
{
    selectedPiece = nullptr;
    validMovements.clear();
}

Piece *ChessGame::getPieceByPos(std::string pos)
{
    return board[fromChessPosition(pos)];
}

std::vector<Move *> *ChessGame::getMoves()
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

std::vector<Piece *> *ChessGame::getWhitePieces()
{
    return &whitePieces;
}

std::vector<Piece *> *ChessGame::getBlackPieces()
{
    return &blackPieces;
}

std::vector<Piece *> *ChessGame::getWhitePiecesKilled()
{
    return &whitePiecesKilled;
}

std::vector<Piece *> *ChessGame::getBlackPiecesKilled()
{
    return &blackPiecesKilled;
}

void ChessGame::passTurn()
{
    turn = !turn;
}

bool ChessGame::getIfSelectingPromoted()
{
    return selectingPromoted;
}

char ChessGame::getPieceType(int pos)
{

    switch (pos)
    {
    case 0:
        return turn ? 'Q' : 'q';
        break;
    case 1:
        return turn ? 'N' : 'n';
        break;
    case 2:
        return turn ? 'R' : 'r';
        break;
    case 3:
        return turn ? 'B' : 'b';
        break;
    default:
        return '\0';
        break;
    }
}

bool *ChessGame::getTurn()
{
    return &turn;
}