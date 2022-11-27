#include "partida.hpp"

Partida::Partida(User *usuario_blancas, User *usuario_negras, sf::Font* font)
{
    this->usuario_blancas = usuario_blancas;
    this->usuario_negras = usuario_negras;
    this->fecha = time(0);

    this->gameInfo = new GameInfo(usuario_blancas, usuario_negras, &jugadas, font);

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

    target.clear(sf::Color::Black);

    for (int i = 0; i < 64; i++)
    {
        target.draw(m_boardSquares[i]);
    }

    for (int i = 0; i < 16; i++)
    {
        target.draw(*piezas_blanco[i]);
        target.draw(*piezas_negro[i]);
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
        jugadas.push_back(j);
        Pieza *pieza = j->getPieza();

        // quitamos movimiento del antiguo
        tablero[pieza->getPos()] = nullptr;

        // movemos la pieza
        pieza->move(j->getNewPos());
        // ponemos el nuevo
        tablero[pieza->getPos()] = pieza;
    }

    // si no se puede hacer se devuelve false y no se le pasa el turno al otro jugador

    return is_aplicable;
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
