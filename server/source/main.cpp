#include <iostream>
#include <fstream>
#include <string>
#include "../headers/json.hpp"

using json = nlohmann::json;

int main()
{
    std::ifstream f("data/data.json");
    json data = json::parse(f);

    // Access the values existing in JSON data
    std::string name = data.value("name", "not found");
    int64_t grade = data.value("grade", -1);
    // Access a value that does not exist in the JSON data
    std::string email = data.value("email", "not found");

    // Print the values
    std::cout << "Name : " << name << std::endl;
    std::cout << "Grade : " << grade << std::endl;
    std::cout << "Email : " << email << std::endl;

    return 0;
}