#include "partida.hpp"

Partida::Partida(User *usuario_blancas, User *usuario_negras, sf::Font *font)
{
    this->usuario_blancas = usuario_blancas;
    this->usuario_negras = usuario_negras;
    this->fecha = time(0);

    this->gameInfo = new GameInfo(usuario_blancas, usuario_negras, &jugadas, font);

    turn = true;
    selected = false;
    selectedPiece = nullptr;

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

            m_boardSquares[j + (i * 8)].setPosition(sf::Vector2f(j * 96.f, i * 96.f));
            m_boardSquares[j + (i * 8)].setSize(sf::Vector2f(96.f, 96.f));
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

bool Partida::aplicarJugada(Jugada *j)
{

    // se mira si se puede hacer (la Piezq tiene movimeinto, no hay jaque)

    // si se puede hacer, se mueve, se actualiza el tablero y se devuelve true (y se mete a la lista de jugadas)

    bool is_aplicable = true;

    if (is_aplicable)
    {
        std::cout << 1 << std::endl;
        jugadas.push_back(j);
        Pieza *pieza = j->getPieza();

        std::cout << 2 << std::endl;
        // quitamos movimiento del antiguo
        std::cout << pieza->getPos() << std::endl;
        tablero[pieza->getPos()] = nullptr;
        
        // si ya habia una pieza, se come y se elimina
        int nuevaPos = j->getNewPos();
        Pieza* pieza_enemiga = tablero[nuevaPos];
        if (pieza_enemiga != nullptr && pieza_enemiga->getColor() != pieza->getColor())
        {
            if (pieza_enemiga->getColor()) // blanca
            {
                for (int i = 0; i < 16; i++)
                {
                    if (piezas_blanco[i]->getPos() == nuevaPos)
                    {
                        piezas_blanco.erase(piezas_blanco.begin() + i);
                    }
                }
            }
            else
            {
                for (int i = 0; i < 16; i++)
                {
                    if (piezas_negro[i]->getPos() == nuevaPos)
                    {
                        piezas_negro.erase(piezas_negro.begin() + i);
                    }
                }

            }
            delete tablero[nuevaPos];
        }

        // for (auto const &i : piezas_blanco)
        // {
        //     cout<<i->getNombre()<<endl;
        // }
        // for (auto const &i : piezas_negro)
        // {
        //     cout<<i->getNombre()<<endl;
        // }

        // movemos la pieza
        pieza->move(nuevaPos);
        

        // ponemos el nuevo
        tablero[nuevaPos] = pieza;
    }

    // si no se puede hacer se devuelve false y no se le pasa el turno al otro jugador

    return is_aplicable;
}

bool Partida::selectPiece(int pos)
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
        return selected;
    }

    // movimientos disponibles
    createMovesSquares();

    return selected;
}

bool Partida::isSelected()
{
    return selected;
}

void Partida::createMovesSquares()
{

    if (selectedPiece == nullptr)
        return;

    possibleMovesSquares.clear();

    for(int i=0; i<selectedPiece->calcularMovimiento().size();i++){
        sf::RectangleShape tmp;
        tmp.setPosition(sf::Vector2f((selectedPiece->calcularMovimiento().at(i) % 8) * 96.f , (7-(selectedPiece->calcularMovimiento().at(i) / 8)) * 96.f));
        tmp.setSize(sf::Vector2f(96.f, 96.f));
        tmp.setFillColor(sf::Color(0x66b4cc50));
        possibleMovesSquares.push_back(tmp);
    }

    sf::RectangleShape tmp;
    tmp.setPosition(sf::Vector2f((selectedPiece->getPos() % 8) * 96.0f, (7 - (selectedPiece->getPos() / 8)) * 96.0f));
    tmp.setSize(sf::Vector2f(96.0f, 96.0f));
    tmp.setFillColor(sf::Color(0x00000000));
    tmp.setOutlineColor(sf::Color::Red);
    tmp.setOutlineThickness(-4.f);
    possibleMovesSquares.push_back(tmp);
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

void Partida::moveSelected(int pos)
{
    bool validMove = false;

    if (validMove)
    {

    }

    if ((selectedPiece == nullptr) || !selected) // Probably doesnt need both
        return;

    // Check pos with the Piece's possibleMoves
    // for(int i=0;i<selectedPiece->getPossibleMoves().size();i++){
    //     if(pos == selectedPiece->getPossibleMoves().at(i)){
    //         validMove = true;
    //         break;
    //     }
    // }

    // if(validMove){

    //     // If Castling Move
    //     if((selectedPiece->getType() == 'K') && (!selectedPiece->getMoved())){
    //         if(selectedPiece->getPlayer()){ // If white
    //             // whitePieces[0] Bot Left Rook, whitePieces[7] Bot Right Rook
    //             if(pos == 62)
    //                 whitePieces[7].setPosition(61);
    //             else if(pos == 58)
    //                 whitePieces[0].setPosition(59);
    //         }
    //         else{ // If Black
    //             // blackPieces[7] Top Left Rook, blackPieces[0] Top Right Rook
    //             if(pos == 6)
    //                 blackPieces[0].setPosition(5);
    //             else if(pos == 2)
    //                 blackPieces[7].setPosition(3);
    //         }
    //     }

    //     // If Pawn double move (set en passant)
    //     // White pawn -16, Black pawn +16
    //     if((selectedPiece->getType() == 'P')){
    //         if(!selectedPiece->getMoved()){
    //             if(pos == (selectedPiece->getPosition() - 16)){
    //                 selectedPiece->setEnPassant(selectedPiece->getPosition() - 8);
    //             }
    //             else if(pos == (selectedPiece->getPosition() + 16)){
    //                 selectedPiece->setEnPassant(selectedPiece->getPosition() + 8);
    //             }
    //         }
    //         else{
    //             for(int i=0; i<16; i++){
    //                 if(playerTurn){
    //                     if(pos == blackPieces[i].getEnPassant())
    //                         blackPieces[i].setPosition(pos);
    //                 }
    //                 else{
    //                     if(pos == whitePieces[i].getEnPassant())
    //                         whitePieces[i].setPosition(pos);
    //                 }
    //             }
    //         }
    //     }
    //     if(selectedPiece->getMoved()){
    //         for(int i=0; i<16; i++){
    //             whitePieces[i].setEnPassant(-1);
    //             blackPieces[i].setEnPassant(-1);
    //         }
    //     }

    // selectedPiece->move(pos);
    Jugada *j = new Jugada(selectedPiece, pos);
    aplicarJugada(j);

    turn = !turn;

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

    selectedPiece = nullptr;
    selected = false;

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