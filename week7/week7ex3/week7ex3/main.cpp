#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

template < typename Iterator, typename UnaryFunction >
UnaryFunction parallel_for_each(Iterator first, Iterator last, UnaryFunction f)
{
    const std::size_t length = std::distance(first, last);

    const std::size_t max_size = 25;

    if (length <= max_size)
    {
        return std::for_each(first, last, f);
    }
    else
    {
        Iterator middle = first;
        std::advance(middle, length / 2);

        std::future < UnaryFunction > first_half_result =
            std::async(parallel_for_each < Iterator, UnaryFunction >, first, middle, f);
        UnaryFunction second_half_result = parallel_for_each(middle, last, f);
        return first_half_result.get();;
    }
}

int main(int argc, char ** argv)
{
    std::vector < int > v(100);

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
