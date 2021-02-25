#include <iostream>
#include <random>
#include <vector>
#include <set>
#include <fstream>
#include <iterator>

std::size_t hash_for_double(double n, double a = 0, double b = 100)
    {
        auto hash_code = std::size_t((1/(b-a) * n - a/(b - a))*3413117);
        return hash_code;
    }


int main()
{
    std::default_random_engine dre;
    std::uniform_real_distribution<double> myrandom(0, 100);

    std::vector<std::size_t> v(100);
    std::vector<double> v1(100);
    
    for (int i=0; i<100; ++i)
    {
        v1[i] = myrandom(dre);
    
    }
   
    for (int i=0; i<100; ++i)
    {
        v[i] = hash_for_double(v1[i]);
    }

    auto i = 0;
    for(i = 0; i < v.size(); ++i)
    {
        for(auto j = i + 1; j < v.size(); ++j)
        {
           if(v[i] == v[j])
            {
                std::cout << "This Function is not deterministic" << std::endl;
            }
              
        }
    }

    for (int i=0; i<100; ++i)
    {
        v[i] = hash_for_double(v1[i])%64;
    }

    std::set<std::size_t> set;
    std::size_t num_of_collisions;

    for (auto N = 10; N < v.size(); N+=10)
    {
        for(i = 0; i < N; ++i)
        {
            for (auto j = i+1; j < N; ++j)
            {
                if(v[i] == v[j])
                {
                    set.insert(v[i]);
                }
            }
        }
        num_of_collisions = N - set.size();
        std::cout << N << " "  << num_of_collisions << std::endl;
    }
    

   //This funstion is:
   //1. deterministic
   //2. fast calculating
   //3. linear relationship

    

    return 0;
}
