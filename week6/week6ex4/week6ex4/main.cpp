#include <filesystem>
#include <fstream>
#include <iomanip>
#include <istream>
#include <locale>
#include <ostream>
#include <iostream>

#include "json.hpp"

using nlohmann::json;

struct Personality
{
    std::string first_name;
    std::string last_name;
    std::size_t phone_number;
    std::string address;
};

// Personality enterPerson(Personality p)
// {
//     std::cout << "Enter name: ";
//     std::cin >> p.first_name;
//     std::cout << std::endl;
//     std::cout << "Enter surname: ";
//     std::cin >> p.last_name;
//     std::cout << std::endl;
//     std::cout << "Enter phone number: ";
//     std::cin >> p.phone_number;
//     std::cout << std::endl;
//     std::cout << "Enter address: ";
//     std::cin >> p.address;
//     std::cout << std::endl;
//     return p;
// }

void to_JSON(Personality p, std::ofstream& f)
{
    std::cout << "Enter name: ";
    std::cin >> p.first_name;
    std::cout << std::endl;
    std::cout << "Enter surname: ";
    std::cin >> p.last_name;
    std::cout << std::endl;
    std::cout << "Enter phone number: ";
    std::cin >> p.phone_number;
    std::cout << std::endl;
    std::cout << "Enter address: ";
    std::cin >> p.address;
    std::cout << std::endl;
     
    json j;
    j["first_name"] = p.first_name;
    j["last_name"] = p.last_name;
    j["phone"] = p.phone_number;
    j["address"] = p.address;
    
    f << "{" << std::endl;
    for (auto & element : j.items())
    {
        f << element.key() << " : " << element.value() << '\n';
    }
    f << "}" << std::endl;
}
int main()
{
    Personality A;
    Personality B;
    Personality C;

    std::ofstream f1("hw6e4_P/person1.txt");
    to_JSON(A, f1);
    std::ofstream f2("hw6e4_P/person2.txt");
    to_JSON(A, f2);
    std::ofstream f3("hw6e4_P/person3.txt");
    to_JSON(A, f3);
    
    return 0;
}
