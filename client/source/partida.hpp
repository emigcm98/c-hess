// #define A 0
// #define B 1
// #define C 2
// #define D 3
// #define E 4
// #define F 5
// #define G 6
// #define H 7

#include <iostream>
#include "user.hpp"
#include <string>
#include <ctime>
#include <list>

#include "jugada.hpp"


// Rey = R, Dama = D, Caballo = C, Alfil = A, Torre = T Peon = P 
// Blanco b, Negro n
// Nada = NULL

//typedef Pieza* (&tablero)[8][8];

using namespace std;

enum Resultado { //dividir entre 2 en la fórmula
    BLANCAS = 2,
    TABLAS = 1,
    NEGRAS = 0
};

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <array>

class Partida : public sf::Drawable
{
    // private data member
private:
    User* usuario_blancas;
    User* usuario_negras;
    vector<Pieza*> piezas_blanco;
    vector<Pieza*> piezas_negro;
    list<Jugada*> jugadas;
    time_t fecha;
    std::array<sf::RectangleShape, 64> m_boardSquares;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
    // public member function
public:
    Partida(User* usuario_blancas, User* usuario_negras);
    // getters
    //tablero tablero = {NULL}; // posicion actual de las piezas, tras la última jugada, cambiar con aplicarJugada
    //Pieza*** getTablero();
    //Pieza* tablero[8][8];
    Pieza* tablero[64];
    void load(sf::Color col1 = sf::Color::White, sf::Color col2 = sf::Color::Black);
    
    Pieza* getPiezaByPos(std::string pos);
    list<Jugada*> getJugadas();
    // setters
    void setResultado(Resultado r); // aplica los cambios a los jugadores
    // methods
    void mostrarTablero();
    bool aplicarJugada(Jugada* j); // aplica los cambios al tablero
    bool isJaque();
    bool isJaqueMate();
};