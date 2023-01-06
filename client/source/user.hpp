#pragma once

#include <iostream>
#include <string>
#include <ctime>

class User
{
    // private data member
private:
    std::string username;
    std::string password;
    struct tm birthdate;
    
    double elo;
    int k; // 10, 15, 20, 30, 40

public:
    User(std::string username, std::string password, struct tm birthdate, double elo = 1200.0);
    ~User();
    // getters
    int getElo();
    std::string getUsername();
    // setters
    void setElo(double elo);
    double calculateNewElo(double opponentElo, float result); // returns the difference

    // methods
    int calculateAge();
    bool isBirthday();

};