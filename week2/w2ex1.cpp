#include <iostream>
#include <vector>


int main()
{
    std::vector<int> v;
    for(auto i = 0; i < 1; ++i)
    {
        v.push_back(i);
    }
    for(auto i = 0; i < 10; ++i)
    {
        v.push_back(i);
    }
    for(auto i = 0; i < 20; ++i)
    {
        v.push_back(i);
    }
      for(auto i = 0; i < 50; ++i)
    {
        v.push_back(i);
    }
    std::cout << "size of vector: "  << v.size() << std::endl;
    std::cout << "capacity: " << v.capacity() << std::endl;
    
    std::vector<int> v1;

    v1.reserve(3);
    for(auto i = 0; i < 20; ++i)
    {
        v1.push_back(i);
    }

    std::cout << "size of vector: "  << v1.size() << std::endl;
    std::cout << "capacity: " << v1.capacity() << std::endl;

    std::vector<int> v2(50000000, 0);
    v2.push_back(1);
    std::cout << "size of vector: "  << v2.size() << std::endl;
    std::cout << "capacity: " << v2.capacity() << std::endl;
    return 0;
    
    std::vector<int> v3(5000000000000, 0);
    v3.push_back(1);
    std::cout << "size of vector: "  << v3.size() << std::endl;
    std::cout << "capacity: " << v3.capacity() << std::endl;
    return 0;
}

/* Если сразу не выделить определенный размер для вектора и не заполнить его push_back(), то емкость вектора будет больше или равно числу элементов вектора и обязательно кратно 2 (т. е. для одного элемента емкость будет равна 1, для двух-два, для трех-четыре и так далее). В случае, когда недостаточно памяти для размещения элементов, емкость увеличивается на 2 до тех пор, пока емкость не станет больше, чем количество элементов вектора.
2. В случае, когда емкость выделяется с помощью reserve() и не хватает места для размещения новых элементов, то выделенная емкость умножается на 2 до тех пор, пока не станет больше или равно числу элементов вектора.
3. Когда выполняется предельный случай (в моем случае сначала до миллиарда, потом ничего не выводится), то все равно умножается на 2, пока не останется больше миллиарда (когда более миллиарда записей пишут "terminate вызывается после броска экземпляра 'std :: bad_alloc'
what (): std :: bad_alloc
 Aborted (core dumped)"") */
   
