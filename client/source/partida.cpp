#include "partida.hpp"

Partida::Partida(User *usuario_blancas, User *usuario_negras)
{
    this->usuario_blancas = usuario_blancas;
    this->usuario_negras = usuario_negras;
    this->fecha = time(0);

    // tablero vacio
    for (int j = 0; j < 8; j++){
        for (int i = 0; i < 8; i++){
            tablero[i][j] = nullptr;
        }
    }

    // peones 
    for (int i = 0; i < 8; i++){

        tablero[i][1] = new Peon(i, 1, true);
        tablero[i][6] = new Peon(i, 6, false);
    }

    // alfiles
    tablero[2][0] = new Alfil(2, 0, true);
    tablero[5][0] = new Alfil(5, 0, true);
    tablero[2][7] = new Alfil(2, 7, false);
    tablero[5][7] = new Alfil(5, 7, false);

    // caballos
    tablero[1][0] = new Caballo(1, 0, true);
    tablero[6][0] = new Caballo(6, 0, true);
    tablero[1][7] = new Caballo(1, 7, false);
    tablero[6][7] = new Caballo(6, 7, false);

    // torres
    tablero[0][0] = new Torre(0, 0, true);
    tablero[7][0] = new Torre(7, 0, true);
    tablero[0][7] = new Torre(0, 7, false);
    tablero[7][7] = new Torre(7, 7, false);

    // rey
    tablero[4][0] = new Rey(4, 0, true);
    tablero[4][7] = new Rey(4, 7, true);

    // dama
    tablero[3][0] = new Dama(3, 0, true);
    tablero[3][7] = new Dama(3, 7, true);
}

bool Partida::aplicarJugada(Jugada *j)
{

    // se mira si se puede hacer (la Piezq tiene movimeinto, no hay jaque)

    // si se puede hacer, se mueve, se actualiza el tablero y se devuelve true (y se mete a la lista de jugadas)

    bool is_aplicable = true;

    if (is_aplicable)
    {
        jugadas.push_front(j);
        Pieza *pieza = j->getPieza();
        
        // quitamos movimiento del antiguo
        tablero[pieza->getFile()][pieza->getCol()] = nullptr;

        // movemos la pieza
        pieza->move(j->getNewFile(), j->getNewCol());
        // ponemos el nuevo
        tablero[pieza->getFile()][pieza->getCol()] = pieza;
    }

    // si no se puede hacer se devuelve false y no se le pasa el turno al otro jugador

    return is_aplicable;
}

void Partida::mostrarTablero()
{
    for (int j = 0; j < 8; j++){
        for (int i = 0; i < 8; i++){
            if (tablero[i][j] == nullptr){
                cout << "X" << " ";
            }
            else {
                // tablero[i][j]->getNombre()
                cout << tablero[i][j]->getNombre() << " ";
            }
        }
        cout << endl;
    }
    cout << endl;
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

Pieza* Partida::getPiezaByIndex(int file, int col)
{
    return tablero[file][col];
}

list<Jugada*> Partida::getJugadas()
{
    return jugadas;
}
