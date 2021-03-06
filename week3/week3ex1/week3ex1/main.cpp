#include <iostream>
#include <set>
#include <vector>
#include <array>
#include <iomanip>
#include <algorithm>
#include <random>

#include "Timer.h"


int main()
{
    
    std::vector<int> v;
    std::set<int> s;
//    std::array<int, 1000000> a;

    Timer tv("vector");
    std::default_random_engine dre;
    v.reserve(10000000);
    for (auto i = 9999999 ; i >= 0; --i)
    {
        v.push_back(i);
    }
    tv.pause();

    std::shuffle(std::begin(v), std::end(v), dre);

    Timer ts("set");
    for(auto i = 0; i < v.size(); ++i)
    {
        s.insert(v[i]);
    }
    ts.pause();

    // Timer ta("array"); //after 10000000 array overflow occurs
    // for(auto i = 0; i < std::size(v); ++i)
    // {
    //     a[i] = v[i];
    // }
    // std::sort(std::begin(a), std::end(a));
    // ta.pause();

    tv.contin();
    std::sort(std::begin(v), std::end(v));
    tv.pause();

    std::cout << "Number of elements: " << v.size() << std::endl;
    // std::cout << std::setw(25) << std::left << ta.name() << std::setw(40) << ta.drtn() << std::endl;
    std::cout << std::setw(25) << std::left << tv.name() << std::setw(40) << tv.drtn() << std::endl;
    std::cout << std::setw(25) << std::left << ts.name() << std::setw(40) << ts.drtn() << std::endl;
   
    return 0;
}
