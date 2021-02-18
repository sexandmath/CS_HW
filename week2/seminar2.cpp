//
//  main.cpp
//  seminar2
//
//  Created by Pavlova Valeria on 13.02.2021.
//

#include <iostream>
#include <list>

int main(int argc, const char ** argv) {
    // insert code here...
    std::list <int> l1, l2;
    for (auto i = 1; i<6; ++i)
    {
        l1.push_back(i);
        l2.push_front(i);
    }
    
    l1.splice(std::next(std::begin(l1), 2), //сдвигает на 2 элемента вперед в л1, стаивт перед 3 элементом
              l2, //перед кем
              std::next(std::begin(l2), 1), //начало вырезаемого
              std::prev(std::end(l2), 1)); //конец вырезаемого
    
    for (auto x : l1)
    {
        std::cout << x << std::endl;
    }
    
    std::cout<< std::endl;
    for (auto x : l2)
    {
        std::cout << x << ''; //чтобы пробел был между цифрами
        
    }
    
    system("pause");
    
    return EXIT_SUCCESS;
    
    
}
