#pragma once

#include <iostream>
#include <string>
#include <ctime>

// DAO Object

class UserDAO
{
    // private data member
private:
    int id;
    std::string username;
    std::string password;
    std::string token;
    float elo;
    std::string birthdate;
    std::string email;
    std::string name;
    
    int k; // 10, 15, 20, 30, 40

public:
    UserDAO(int id, std::string username, std::string password, std::string token, float elo, std::string birthdate, std::string email, std::string name);
    ~UserDAO();
    // getters
    int getId();
    std::string getUsername();
    std::string getPassword(); // encrypted
    std::string getToken();
    float getElo();
    std::string getBirthdate();
    std::string getEmail();
    std::string getName();
    // setters
    // methods
    //int calculateAge();
    float calculateNewElo(float opponentElo, float result); // returns the difference
    //bool isBirthday();

};