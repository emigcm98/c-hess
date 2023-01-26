#include <mysql++/mysql++.h>
#include "userDAO.hpp"

class MySQLConnector
{

private:
    std::string host;
    std::string db;
    std::string user;
    std::string pass;
    int puerto;
    mysqlpp::Connection conn;

public:
    MySQLConnector(std::string host, std::string db, std::string user, std::string pass, int puerto);
    void connect();
    bool isConnected();

    bool createUser(std::string username, std::string email, std::string password, std::string name, std::string birthdate, std::string gender);
    UserDAO* login(std::string username, std::string password); // return token
    int getUserId(std::string token);
    int getElo(int id);
    bool checkIfUserExists(std::string username);
    bool createGame(UserDAO* whitePlayer, UserDAO* blackPlayer, std::string pgn, std::string result, std::string timeControl, std::string opening);
    //std::string login(std::string email, std::string password);
};