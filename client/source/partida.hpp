#include <iostream>
#include "user.hpp"
#include <string>
#include <ctime>

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

#include "gameinfo.hpp"

class Partida : public sf::Drawable
{
    // private data member
private:
    User* usuario_blancas;
    User* usuario_negras;
    vector<Pieza*> piezas_blanco;
    vector<Pieza*> piezas_negro;
    vector<Jugada*> jugadas;
    GameInfo* gameInfo;
    time_t fecha;
    std::array<sf::RectangleShape, 64> m_boardSquares;
    std::vector<sf::RectangleShape> possibleMovesSquares;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
    Pieza* selectedPiece;
    bool selected; // something selected ?
    bool turn; //true white false black
    // public member function
public:
    Partida(User* usuario_blancas, User* usuario_negras, sf::Font* font);
    Pieza* tablero[64];
    // getters
    bool isJaque();
    bool isJaqueMate();
    
    Pieza* getPiezaByPos(std::string pos);
    vector<Jugada*> getJugadas();
    // setters
    void setResultado(Resultado r); // aplica los cambios a los jugadores
    // methods
    void load(sf::Color col1 = sf::Color::White, sf::Color col2 = sf::Color::Black);
    void mostrarTablero();
    bool aplicarJugada(Jugada* j); // aplica los cambios al tablero
    bool selectPiece(int pos);
    bool isSelected();
    void moveSelected(int pos);
    void createMovesSquares();
    std::vector<int> filterValidMovements(Pieza *p);
};