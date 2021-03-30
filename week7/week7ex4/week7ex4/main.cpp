
#include <algorithm>
#include <cmath>
#include <execution>
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>

#include "Timer.hpp"

int main()
{

    auto f =[](const auto elem, const auto el)
    {
        auto res = 0.0;
        for(auto i = 0; i < 20; ++i)
        {
            res= asin(elem + el +res);
        }
        return res;
    };
    
    std::ofstream fout("hw7e4.csv");
    fout << "n,t1,t2,t3,t4"<< std::endl;
       for(auto i = 10; i <= 100000000; i*=5)
       {
            std::vector<double> v1(i);
            std::iota(std::begin(v1), std::end(v1), 1.0);
            std::vector<double> v2(i);
            std::iota(std::begin(v2), std::end(v2), 1.0);
            
            Timer t1("parallel for_each");
            std::for_each(std::execution::par, std::begin(v1), std::end(v1), [](auto & elem){
                elem = cos(elem);
             });
            t1.pause();
            
            Timer t2("standart for_each");
            std::for_each(std::begin(v2), std::end(v2), [](auto & elem){
                elem = cos(elem);});
            t2.pause();

            Timer t3("inclusive_scan");
            std::inclusive_scan(std::execution::par, std::begin(v1), std::end(v1), std::begin(v1), [](const auto & elem, const auto & e){
                return cos(elem + e);});
            t3.pause();

            Timer t4("partial_sum");
            std::partial_sum(std::begin(v2), std::end(v2), std::begin(v2), [](const auto & elem, const auto & e){
               return cos(elem + e);});
            t4.pause();

            fout << i << ',' << t1.drtn() << ',' << t2.drtn() << ',' << t3.drtn() << ',' << t4.drtn() <<  std::endl;
       }
        
        std::ofstream fou("hw7e4_2.csv");
        fou << "n,t5,t6"<< std::endl;
        for(auto i = 10; i <= 100000000; i*=5)
       {
            std::vector<double> v1(i);
            std::iota(std::begin(v1), std::end(v1), 1.0);
            std::vector<double> v2(i);
            std::iota(std::begin(v2), std::end(v2), 1.0);
        Timer t5("transform_reduce");
            std::transform_reduce(std::execution::par, std::begin(v1), std::end(v1), std::begin(v2), 0, f, std::multiplies()) << '\n';
            t5.pause();

            Timer t6("inner_product");
             std::inner_product(std::begin(v1), std::end(v1), std::begin(v2), 0, f, std::multiplies());
            t6.pause();
            fou << i << ',' << t5.drtn() << ',' << t6.drtn() <<  std::endl;
       }
    return 0;
}
