#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iterator>

bool is_prime(int n)
{
    if(n == 1)
    {
        return false;
    }
    for(auto i = 2;i <= sqrt(n); ++i)
        if(n % i == 0)
            return false;
    return true;
}
    int my_random()
    {
        std::default_random_engine dre(static_cast <std::size_t> (std::chrono::system_clock::now().time_since_epoch().count()));
        std::uniform_int_distribution<> my_random(0, 100);
        return my_random(dre);
    }

int main()
{
    //create sequence of 10 elements
    std::vector<int> p1(10);
    //first way
    // int e = 1;
    // std::for_each(std::begin(p1), std::end(p1), [&e](int& elem){
    //     elem += e;
    //     ++e;});

    //second way
    std::iota(std::begin(p1), std::end(p1), 1);

    //add nubers with help of std::cin
    std::copy(
        std::istream_iterator < int > (std::cin),
        std::istream_iterator < int > (),
        std::back_inserter(p1));
 
    // shuffle sequence elements
    //first way
    std::default_random_engine dre(static_cast <std::size_t> (std::chrono::system_clock::now().time_since_epoch().count()));
    std::shuffle(std::begin(p1), std::end(p1), dre);

    //second way
    // std::random_shuffle(std::begin(p1), std::end(p1));

    //delete duplicates
    std::stable_sort(std::begin(p1), std::end(p1));
    p1.erase(std::unique(std::begin(p1), std::end(p1)), std::end(p1));

    for (auto i = 0; i < std::size(p1); ++i){
        std::cout << p1[i] << " ";
    }
    std::cout<< std::endl;

    //counting the number of odd numbers
    std::cout << "Number of odd numbers: " << std::count_if(std::begin(p1), std::end(p1), [](int elem){return elem % 2 != 0;} ) << std::endl;

    //min and max elements of sequence p1
    const auto [min, max] = std::minmax_element(std::begin(p1), std::end(p1));
    std::cout << "min = " << *min << " max = " << *max << std::endl;

    // search for a prime number
    auto pos = std::find_if(std::begin(p1), std::end(p1), is_prime);
    if (pos != std::end(p1))
    {
        std::cout << "Prime number is " << *pos << std::endl;
    }
    else
    {
        std::cout << "No prime numbers" << std::endl;
    }
    //replacing all elements of the sequence with squares
    //first way
    // std::for_each(std::begin(p1), std::end(p1), [](int & elem){ elem*=elem; });
   
    //second way
    std::transform(std::begin(p1), std::end(p1), std::begin(p1), [](int elem) { return elem *= elem; });
    
    //creat second sequence p2
    std::vector<int> p2(std::size(p1), 0);

    //first way
    std::generate(std::begin(p2), std::end(p2), my_random);
    
    //second way
    // for(auto i = 0; i < std::size(p2); ++i)
    // {
    //     p2[i] = my_random(dre);
    // }


    //sum of sequence elements
    std::cout << "sum: " << std::accumulate(std::begin(p2), std::end(p2), 0) << std::endl;

    //replace numbers with 1
    std::fill_n(std::begin(p2), 5, 1);

    // sequencing p3
    std::vector<int> p3(std::size(p1));
    std::transform(std::begin(p1), std::end(p1),
                   std::begin(p2), std::begin(p3),
                   std::minus<int>());

    //replacing negative values with zero
    std::replace_if(std::begin(p3), std::end(p3), [](int& elem){ return elem < 0; }, 0);

    //removing zero elememts
    std::remove_if(std::begin(p3), std::end(p3), [](int& elem){ return elem == 0; });
    
    // changing the order of the elements
    std::reverse(std::begin(p3), std::end(p3));
    

    //finding the 3 largest elements
    std::partial_sort(std::begin(p3), std::next(std::begin(p3))+2, std::end(p3), std::greater<>());
    std::cout << "Top 3 elements" << std::endl;
    std::copy(std::begin(p3), std::next(std::begin(p3))+2, std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    //sorting sequences p1 and p2
    std::sort(std::begin(p1), std::end(p1));
    std::sort(std::begin(p2), std::end(p2));

    // sequencing p4
    std::vector<int> p4(2*std::size(p1));
    std::merge(std::begin(p1), std::end(p1),
               std::begin(p2), std::end(p2),
               std::begin(p4));

    // defining the range
    auto range = std::equal_range(std::begin(p4), std::end(p4), 1);
    std::cout << "1 could get position " << std::distance(std::begin(p4), range.first) + 1
              << " up to " <<  std::distance(std::begin(p4), range.second) + 1 << std::endl;

    //sequence output
    std::cout << "p1 : ";
    std::copy(std::begin(p1), std::end(p1), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    std::cout << "p2 : ";
    std::copy(std::begin(p2), std::end(p2), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    
    std::cout << "p3 : ";
    std::copy(std::begin(p3), std::end(p3), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    
    std::cout << "p4 : ";
    std::copy(std::begin(p4), std::end(p4), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    return 0;
} 
