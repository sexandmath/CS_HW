
#include "Timer.h"
#include <array>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <algorithm>
#include <iomanip>
#include <functional>

int main()
{
    std::array<int, 1000> a;
    std::vector<int> v(1000, 0);
    std::deque<int> d(1000,0);
    std::list<int> l;
    std::forward_list<int> fl;

    for (auto i = 0; i < 1000; ++i)
    {
        fl.push_front(i);
    }
    for (auto i = 999; i >=0; --i)
    {
        a[i] = i;
        v[i] = i;
        d[i] = i;
        l.push_back(i);
    }
    Timer array("array");                               // 1. Array
    std::sort(begin(a), end(a));                        // 2. Vector/forward_list
    array.pause();                                      // 3. deque
                                                        // 4. list
    Timer vector("vector");
    std::sort(begin(v), end(v));
    vector.pause();

    Timer deque("deque");
    std::sort(begin(d), end(d));
    deque.pause();

    Timer list("list");
    l.sort();
    list.pause();

    Timer f_list("forward_list");
    fl.sort();
    f_list.pause();


    std::cout << std::setw(25) << std::left << array.name() << std::setw(40) << array.drtn() << std::endl;
    std::cout << std::setw(25) << std::left << vector.name() << std::setw(40) << vector.drtn() << std::endl;
    std::cout << std::setw(25) << std::left << deque.name() << std::setw(40) << deque.drtn() << std::endl;
    std::cout << std::setw(25) << std::left << list.name() << std::setw(40) << list.drtn() << std::endl;
    std::cout << std::setw(25) << std::left << f_list.name() << std::setw(40) << f_list.drtn() << std::endl;
    return 0;
}

// 1. Array
// 2. Vector/forward_list
// 3. deque
// 4. list
