#include "user.hpp"
#include <math.h> 

User::User(std::string username, std::string password, struct tm birthdate, double elo){
    this->username = username;
    this->password = password;
    this->birthdate = birthdate;
    this->elo = elo;
    k = 30;
}

User::~User()
{
    //std::cout << "deleting user " << username << std::endl;
}

int User::calculateAge()
{

    time_t tNow = time(0);
    struct tm now = *localtime(&tNow);

    now.tm_year += 1900;

    if (now.tm_year + 1900 - birthdate.tm_year < 0)
    {
        return -1;
    }
    else if (now.tm_mon - birthdate.tm_mon > 0)
    {
        return now.tm_year - birthdate.tm_year;
    }
    else if (now.tm_mon - birthdate.tm_mon < 0)
    {
        return now.tm_year - birthdate.tm_year - 1;
    }
    else if (now.tm_mon - birthdate.tm_mon == 0)
    {
        if (now.tm_mday - birthdate.tm_mday >= 0)
        {
            return now.tm_year - birthdate.tm_year;
        }
        else
        {
            return now.tm_year - birthdate.tm_year - 1;
        }
    }
    else
    {
        return -1;
    }
}

bool User::isBirthday() {

    time_t tNow = time(0);
    struct tm now = *localtime(&tNow);

    now.tm_year += 1900;
    if (now.tm_year >= birthdate.tm_year && now.tm_mon == birthdate.tm_mon && now.tm_mday == birthdate.tm_mday)
    {
        return true;
    }
    else
    {
        return false;
    }
       
}

double User::calculateNewElo(double opponentElo, float result)
{
    double expRes = 1/(1+pow(10, (opponentElo - elo)/400));
    std::cout << "Expected result: " << expRes << " (k=" << k << ")." << std::endl;

    double newElo = elo + k * (result-expRes);
    double diff = newElo-elo;
    // actualizamos elo
    elo = newElo;
    return diff;
}

// getters and setters

int User::getElo()
{
    return round(elo); 
}

std::string User::getUsername()
{
    return username;
}