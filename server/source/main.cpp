#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"
#include "mysqlconnector.hpp"

using json = nlohmann::json;

int main()
{
    // std::ifstream f("data/data.json");
    // json data = json::parse(f);

    // // Access the values existing in JSON data
    // std::string name = data.value("name", "not found");
    // int64_t grade = data.value("grade", -1);
    // // Access a value that does not exist in the JSON data
    // std::string email = data.value("email", "not found");

    // // Print the values
    // std::cout << "Name : " << name << std::endl;
    // std::cout << "Grade : " << grade << std::endl;
    // std::cout << "Email : " << email << std::endl;

    int puerto = 33306;
    MySQLConnector connector("127.0.0.1", "chessdb", "admin", "admin", puerto);

    if (!connector.isConnected())
    {
        std::cout << "Error al conectar a la base de datos." << std::endl;
        return 1;
    }

    std::cout << "Conexión exitosa!!" << std::endl;
    if (!connector.checkIfUserExists("emigcm98"))
    {
        connector.createUser("emigcm98", "er.garciadelacalera@gmail.com", "newpassword", "Emilio García de la Calera", "1998-10-14", "M");
    }
    if (!connector.checkIfUserExists("eve"))
    {
        connector.createUser("eve", "eve@gmail.com", "newpassword2", "New User 2", "2000-09-16", "M");
    }

    UserDAO *user1 = connector.login("emigcm98", "newpassword");
    std::cout << "token: " << user1->getToken() << std::endl;
    //int id = connector.getUserId(token);
    //int elo = connector.getElo(id);
    std::cout << "userid: " << user1->getId() << std::endl;

    UserDAO *user2 = connector.login("eve", "newpassword2");
    std::cout << "token2: " << user2->getToken() << std::endl;
    // int id2 = connector.getUserId(token2);
    // int elo2 = connector.getElo(id2);
    std::cout << "userid2: " << user2->getId() << std::endl;

    // create game
    bool res = connector.createGame(user1, user2, "1.e4 e5 1-0", "1-0", "5'+2", "gambito de gay");
    if (res)
    {
        std::cout << "partida creada correctamente" << std::endl;
    }
    else {
        std::cout << "no se ha podido crear la partida" << std::endl;
    }

    return 0;
}