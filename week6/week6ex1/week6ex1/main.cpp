
#include <iostream>
#include <fstream>
#include <regex>

std::string readFile(std::ifstream& fin)
{
    return std::string((std::istreambuf_iterator<char>(fin)),
        std::istreambuf_iterator<char>());
}

int main()
{

    std::ifstream fin("test.txt");
    std::string data = readFile(fin);
    
    std::regex pat_out(R"((<< ")? .* (" <<)? .*;)");
    std::regex pattern_comm (R"((/\*(.|[\r\n])*?\*/)|//(.?)*)");

    std::string a = std::regex_replace(data,  pattern_comm, " ");

    std::ofstream("a_test.txt") << a;

    return 0;
}
