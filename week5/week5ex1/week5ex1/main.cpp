#include <iostream>
#include <iomanip>
#include <locale>

long double exchng(long double s)
{
    return s*74.0393;
}

int main()
{
    long double smon;
 
    std::cout.imbue(std::locale("en_US.UTF-8"));
    std::cout << std::showbase
              << "en_US: ";
    std::cin >> std::get_money(smon);

    long double rumon = exchng(smon);
    std::cout.imbue(std::locale("ru_RU.UTF-8"));
    std::cout << std::showbase <<  "ru_RU: " << std::put_money(rumon*100) << std::endl;
 
 return 0;
}
