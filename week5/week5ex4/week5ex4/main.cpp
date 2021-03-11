#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <algorithm>

std::string readFile(std::ifstream& fin)
{
    return std::string((std::istreambuf_iterator<char>(fin)),
        std::istreambuf_iterator<char>());
}

int main()
{
    std::ifstream fin("hw5e3_4.txt");
    std::string data = readFile(fin);
    
    std::regex pattern(R"((([123]?[0-9])\.(1?[0-9]|\w+)\.20[0-9]{2}).([01]\d|2[0-3]):?([0-5]\d):?[0-5]\d)");
    
    std::sregex_iterator beg(std::begin(data), std::end(data), pattern);
    std::sregex_iterator end;
    
    std::ofstream fout("hw5e4_out.txt");
    std::for_each(beg, end, [&](const std::smatch & m)
    {
        fout << m.str() << std::endl;
    });

       

    return 0;
}
