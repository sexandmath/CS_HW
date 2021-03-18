#include <functional>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <chrono>
#include <random>

std::set <std::string> makeRandomWords (std::size_t N, std::size_t length)
{
    std::uniform_int_distribution letter(97, 122);
    std::default_random_engine  e(static_cast <std::size_t> (std::chrono::system_clock::now().time_since_epoch().count()));

    std::set <std::string> words;

    for (std::string str(length, '_'); words.size() <= N; words.insert(str))
        for (auto &c : str)
            c = letter(e);

    return words;
}

int main()
{
    std::set<std::string> strings =  makeRandomWords(20, 10);
    
    std::ofstream fout("hw6e2.txt");
    for(auto c: strings)
    {
        fout << c << std::endl;
    }
    
    std::ifstream fin("hw6e2.txt");

    int ln;
    std::cout << "Enter line number: ";
    std::cin >> ln;
    std::cout << std::endl;

    fin.seekg((ln-1)*11, std::ios::beg);

    std::string c;
    std::getline(fin, c);

    std::cout << c << std::endl;
    return 0;

}
