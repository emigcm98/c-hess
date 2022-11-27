#include "user.hpp"
#include <math.h> 

User::User(std::string username, std::string password, struct tm birthdate){
    this->username = username;
    this->password = password;
    this->birthdate = birthdate;
    elo = 1200.0;
    k = 30;
}

int User::calcular_edad()
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

bool User::is_birthday() {

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

double User::calculate_new_elo(double opponent_elo, float resultado)
{
    double p_esp = 1/(1+pow(10, (opponent_elo - elo)/400));
    std::cout << "Puntuación esperada: " << p_esp << " (k=" << k << ")." << std::endl;

    double new_elo = elo + k * (resultado-p_esp);
    double diff = new_elo-elo;
    // actualizamos elo
    elo = new_elo;
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