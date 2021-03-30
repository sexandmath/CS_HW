#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

template < typename Iterator, typename UnaryFunction >
void parallel_for_each(Iterator first, Iterator last, UnaryFunction f)
{
    const std::size_t length = std::distance(first, last);

    const std::size_t max_size = 1000;

    if (length <= max_size)
    {
        std::for_each(first, last, f);
    }
    else
    {
        Iterator middle = first;
        std::advance(middle, length / 2);

        std::future < void > first_half_result =
            std::async(parallel_for_each < Iterator, UnaryFunction >, first, middle, f);
         parallel_for_each(middle, last, f);
        first_half_result.wait();
    }
}

int main(int argc, char ** argv)
{
    std::vector < int > v(10000000);

    std::iota(v.begin(), v.end(), 1);

     parallel_for_each(v.begin(), v.end(), [](auto & elem){
         ++elem;
     } );
    for(auto e : v)
    {
        std::cout << e << " ";
    }
   
    

    return 0;
}
