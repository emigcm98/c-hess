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

    // public member function
public:
    User(std::string username, std::string password, struct tm birthdate);
    // getters
    int getElo();
    std::string getUsername();
    // setters
    void setElo(double elo);
    double calculate_new_elo(double opponent_elo, float resultado); // returns the difference

    // methods
    int calcular_edad();
    bool is_birthday();

};