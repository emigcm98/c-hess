#include "mysqlconnector.hpp"
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <uuid/uuid.h>
#include <math.h>

std::string generateToken() {
    uuid_t uuid;
    uuid_generate(uuid);
    char token[37];
    uuid_unparse(uuid, token);
    return std::string(token);
}

// std::string encryptPassword(std::string password) {
//     unsigned char hash[SHA256_DIGEST_LENGTH];
//     SHA256_CTX sha256;
//     SHA256_Init(&sha256);
//     SHA256_Update(&sha256, password.c_str(), password.size());
//     SHA256_Final(hash, &sha256);
//     std::stringstream ss;
//     for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
//     {
//         ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
//     }
//     return ss.str();
// }

std::string encryptPassword(std::string password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX *mdctx;
    mdctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(mdctx, password.c_str(), password.size());
    EVP_DigestFinal_ex(mdctx, hash, NULL);
    EVP_MD_CTX_destroy(mdctx);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

// std::string encryptPassword(std::string password) {
//     //Declaración de variables
//     unsigned char hash[SHA256_DIGEST_LENGTH];
//     SHA256_CTX sha256;
//     std::string encryptedPassword;

//     //Inicializamos la estructura de datos de SHA256
//     SHA256_Init(&sha256);

//     //Añadimos el password a la estructura de datos de SHA256
//     SHA256_Update(&sha256, password.c_str(), password.size());

//     //Calculamos el hash
//     SHA256_Final(hash, &sha256);

//     //Convertimos el hash a una cadena hexadecimal
//     for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
//     {
//         encryptedPassword += std::to_string((hash[i] & 0xff) + 0x100, 16).substr(1);
//     }

//     return encryptedPassword;
// }

MySQLConnector::MySQLConnector(std::string host, std::string db, std::string user, std::string pass, int puerto) : host(host), db(db), user(user), pass(pass), puerto(puerto)
{
    connect();
}

void MySQLConnector::connect()
{
    conn.connect(db.c_str(), host.c_str(), user.c_str(), pass.c_str(), puerto);
}

bool MySQLConnector::isConnected()
{
    return conn.connected();
}

bool MySQLConnector::createUser(std::string username, std::string email, std::string password, std::string name, std::string birthdate, std::string gender)
{

    // Generate a unique token for the user
    std::string token = generateToken();

    // Hash the password using a secure hashing algorithm
    std::string encryptedPassword = encryptPassword(password);

    int defaultElo = 1200;
    mysqlpp::Query query = conn.query();
    query << "INSERT INTO users (username, email, password, elo, name, birthdate, gender, token) "
          << "VALUES ('" << username << "', '" << email << "', '" << encryptedPassword << "', " << defaultElo << ", '" << name << "', '" << birthdate  << "', '" << gender << "', '" << token << "')";

    mysqlpp::SimpleResult result = query.execute();

    if (result)
    {
        std::cout << "Usuario creado exitosamente!" << std::endl;
    }
    else
    {
        std::cout << "Error al crear usuario: " << query.error() << std::endl;
    }

    return result;
}

UserDAO* MySQLConnector::login(std::string username, std::string password) {

    // Comprobar si el usuario existe y la contraseña es correcta
    mysqlpp::Query query = conn.query();
    query << "SELECT id, password, elo, birthdate, email, name FROM users WHERE username = '" << username << "'";
    mysqlpp::StoreQueryResult result = query.store();

    if (result.empty()) {
        std::cout << "Usuario o contraseña inválidos" << std::endl;
        return nullptr;
    }

    //std::string hashedPassword = result[0]["password"];
    std::string hashedPassword = result[0]["password"].c_str();

    if (hashedPassword != encryptPassword(password))
    {
        std::cout << "Contraseña incorrecta" << std::endl;
        return nullptr;
    }


    std::string newToken = generateToken();

    query = conn.query();
    query << "UPDATE users SET token = '" << newToken << "' WHERE username = '" << username << "'";
    query.execute();

    // get the User and create it via DAO User object

    int id = result[0]["id"];
    std::string birthdate = result[0]["birthdate"].c_str();
    std::string email = result[0]["email"].c_str();
    std::string name = result[0]["name"].c_str();
    float elo = result[0]["elo"];

    UserDAO* user = new UserDAO(id, username, hashedPassword, newToken, elo, birthdate, email, name);

    return user;
}

int MySQLConnector::getUserId(std::string token)
{
    mysqlpp::Query query = conn.query();
    query << "SELECT id FROM users WHERE token = '" << token << "'";

    mysqlpp::StoreQueryResult result = query.store();

    if (result.empty()) {
        std::cout << "Token inválido" << std::endl;
        return -1;
    }

    return result[0][0];
}

int MySQLConnector::getElo(int id)
{
    mysqlpp::Query query = conn.query();
    query << "SELECT elo FROM users WHERE id = '" << id << "'";

    mysqlpp::StoreQueryResult result = query.store();

    if (result.empty()) {
        std::cout << "Id inválido" << std::endl;
        return -1;
    }

    return result[0][0];
}

bool MySQLConnector::createGame(UserDAO* whitePlayer, UserDAO* blackPlayer, std::string pgn, std::string gameresult, std::string timeControl, std::string opening)
{
    int whitePlayerId = whitePlayer->getId();
    int blackPlayerId = blackPlayer->getId();

    if (whitePlayerId < 1 || blackPlayerId < 1 || whitePlayerId == blackPlayerId)
    {
        std::cout << "Ids incorrectos: " << whitePlayerId << " y " << blackPlayerId << std::endl;
        return false;
    }

    mysqlpp::Query query = conn.query();
    query << "INSERT INTO games (whiteplayer, blackplayer, whiteelo, blackelo, elo_diff, pgn, result, time_control, opening) VALUES ("
        << whitePlayerId << ", " << blackPlayerId << ", " << ceil(whitePlayer->getElo()) << ", " << ceil(blackPlayer->getElo()) << ", " << ceil(whitePlayer->getElo()-blackPlayer->getElo()) << ", " << mysqlpp::quote << pgn << ", " << mysqlpp::quote << gameresult << ", " 
        << mysqlpp::quote << timeControl << ", " << mysqlpp::quote << opening << ")";

    mysqlpp::SimpleResult result = query.execute();

    if (result)
    {
        std::cout << "Partida creada exitosamente!" << std::endl;

        float newWhiteElo = whitePlayer->getElo();
        float newBlackElo = blackPlayer->getElo();

        if (gameresult == "1-0"){
            newWhiteElo += whitePlayer->calculateNewElo(newBlackElo, 1);
            newBlackElo += blackPlayer->calculateNewElo(newWhiteElo, 0);
        }
        else if (gameresult == "0-1")
        {
            newWhiteElo += whitePlayer->calculateNewElo(newBlackElo, 0);
            newBlackElo += blackPlayer->calculateNewElo(newWhiteElo, 1);
        }
        else if (gameresult == "1/2-1/2")
        {
            newWhiteElo += whitePlayer->calculateNewElo(newBlackElo, 0.5);
            newBlackElo += blackPlayer->calculateNewElo(newWhiteElo, 0.5);
        }
        
        query << "UPDATE users SET elo = '" << newWhiteElo << "' WHERE id = '" << whitePlayerId << "'";
        mysqlpp::SimpleResult result = query.execute();

        if (result)
        {
            std::cout << "Actualizado el elo de whitePlayer!" << std::endl;
        }
        else {
            std::cout << "Error al actualizar el elo de whitePlayer" << std::endl;
            return false;
        }

        query << "UPDATE users SET elo = '" << newBlackElo << "' WHERE id = '" << blackPlayerId << "'";
        result = query.execute();

        if (result)
        {
            std::cout << "Actualizado el elo de blackPlayer!" << std::endl;
        }
        else {
            std::cout << "Error al actualizar el elo de blackPlayer" << std::endl;
            return false;
        }
    }
    else
    {
        std::cout << "Error al crear usuario: " << query.error() << std::endl;
        return false;
    }

    return result;
}

bool MySQLConnector::checkIfUserExists(std::string username)
{
    mysqlpp::Query query = conn.query();
    query << "SELECT id FROM users WHERE username = '" << username << "'";

    mysqlpp::StoreQueryResult result = query.store();

    if (result.empty()) {
        return false;
    }
    return true;
}