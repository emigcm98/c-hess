#include "userDAO.hpp"
#include <math.h>

UserDAO::UserDAO(int id, std::string username, std::string password, std::string token, float elo, std::string birthdate, std::string email, std::string name)
 : id(id), username(username), password(password), token(token), elo(elo), birthdate(birthdate), email(email), name(name) {
    k = 30;
}

UserDAO::~UserDAO()
{
    //std::cout << "deleting user " << username << std::endl;
}

// int User::calculateAge()
// {

//     time_t tNow = time(0);
//     struct tm now = *localtime(&tNow);

//     now.tm_year += 1900;

//     if (now.tm_year + 1900 - birthdate.tm_year < 0)
//     {
//         return -1;
//     }
//     else if (now.tm_mon - birthdate.tm_mon > 0)
//     {
//         return now.tm_year - birthdate.tm_year;
//     }
//     else if (now.tm_mon - birthdate.tm_mon < 0)
//     {
//         return now.tm_year - birthdate.tm_year - 1;
//     }
//     else if (now.tm_mon - birthdate.tm_mon == 0)
//     {
//         if (now.tm_mday - birthdate.tm_mday >= 0)
//         {
//             return now.tm_year - birthdate.tm_year;
//         }
//         else
//         {
//             return now.tm_year - birthdate.tm_year - 1;
//         }
//     }
//     else
//     {
//         return -1;
//     }
// }

// bool User::isBirthday() {

//     time_t tNow = time(0);
//     struct tm now = *localtime(&tNow);

//     now.tm_year += 1900;
//     if (now.tm_year >= birthdate.tm_year && now.tm_mon == birthdate.tm_mon && now.tm_mday == birthdate.tm_mday)
//     {
//         return true;
//     }
//     else
//     {
//         return false;
//     }
       
// }

float UserDAO::calculateNewElo(float opponentElo, float result)
{
    float expRes = 1/(1+pow(10, (opponentElo - elo)/400));
    std::cout << "Expected result: " << expRes << " (k=" << k << ")." << std::endl;

    float newElo = elo + k * (result-expRes);
    float diff = newElo-elo;
    // actualizamos elo
    elo = newElo;
    return diff;
}

// getters and setters

int UserDAO::getId()
{
    return id;
}

float UserDAO::getElo()
{
    return elo; 
}

std::string UserDAO::getUsername()
{
    return username;
}

std::string UserDAO::getPassword()
{
    return password;
}

std::string UserDAO::getToken()
{
    return token;
}

std::string UserDAO::getBirthdate()
{
    return birthdate;
}

std::string UserDAO::getEmail()
{
    return email;
}

std::string UserDAO::getName()
{
    return name;
}