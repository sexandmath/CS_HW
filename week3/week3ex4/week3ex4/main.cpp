#include <functional>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <set>
#include <chrono>

std::set <std::string> makeRandomWords (std::size_t N, std::size_t length)
{
    std::uniform_int_distribution letter(97, 122);
    std::default_random_engine  e(static_cast <std::size_t> (std::chrono::system_clock::now().time_since_epoch().count()));

    std::set <std::string> words;

    for (std::string str(length, '_'); words.size() <= N; words.insert(str))
    {
        for (auto &c : str)
        {
            c = letter(e);
        }
        str+='\0';
    }
    return words;
}

unsigned int RSHash(const char* str, unsigned int length)
{
   unsigned int b    = 378551;
   unsigned int a    = 63689;
   unsigned int hash = 0;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = hash * a + (*str);
      a    = a * b;
   }

   return hash;
}

unsigned int JSHash(const char* str, unsigned int length)
{
   unsigned int hash = 1315423911;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash ^= ((hash << 5) + (*str) + (hash >> 2));
   }

   return hash;
}

unsigned int PJWHash(const char* str, unsigned int length)
{
   const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
   const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
   const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
   const unsigned int HighBits          =
                      (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
   unsigned int hash = 0;
   unsigned int test = 0;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = (hash << OneEighth) + (*str);

      if ((test = hash & HighBits) != 0)
      {
         hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
      }
   }

   return hash;
}

unsigned int ELFHash(const char* str, unsigned int length)
{
   unsigned int hash = 0;
   unsigned int x    = 0;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = (hash << 4) + (*str);

      if ((x = hash & 0xF0000000L) != 0)
      {
         hash ^= (x >> 24);
      }

      hash &= ~x;
   }

   return hash;
}

unsigned int BKDRHash(const char* str, unsigned int length)
{
   unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
   unsigned int hash = 0;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = (hash * seed) + (*str);
   }

   return hash;
}

unsigned int SDBMHash(const char* str, unsigned int length)
{
   unsigned int hash = 0;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = (*str) + (hash << 6) + (hash << 16) - hash;
   }

   return hash;
}

unsigned int DJBHash(const char* str, unsigned int length)
{
   unsigned int hash = 5381;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = ((hash << 5) + hash) + (*str);
   }

   return hash;
}

unsigned int DEKHash(const char* str, unsigned int length)
{
   unsigned int hash = length;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
   }

   return hash;
}

unsigned int APHash(const char* str, unsigned int length)
{
   unsigned int hash = 0xAAAAAAAA;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (*str) * (hash >> 3)) :
                               (~((hash << 11) + ((*str) ^ (hash >> 5))));
   }

   return hash;
}

int main()
{
    std::vector <const char*> v;
    std::set <std::string> s = makeRandomWords(1000, 10);
    v.reserve(1000);

    for (const auto & c : s)
    {
        const char * l = c.c_str();
        v.push_back(l);
    }

    std::vector<std::size_t> hc_1(std::size(v));
    std::set<std::size_t> hc;

    
    std::ofstream fout("hw3e4.txt");

   
    fout << "data for RSHash" << std::endl;

    for(auto i = 0; i < std::size(v); ++i)
    {
        hc_1[i] = RSHash(v[i], 10)%64;
    }

    std::size_t i;
    std::size_t num_of_collisions;
    for (auto N = 10; N < std::size(hc_1); N+=10)
    {
        for(i = 0; i < N; ++i)
        {
            for (auto j = i+1; j < N; ++j)
            {
                if(hc_1[i] == hc_1[j])
                {
                    hc.insert(hc_1[i]);
                }
        
            }
        }
        num_of_collisions = N - std::size(hc);
        fout << N << "      " << num_of_collisions << std::endl;
        hc.clear();
    }

    
    
    fout << "data for JSHash" << std::endl;
   
    for(auto i = 0; i < std::size(v); ++i)
    {
        hc_1[i] = JSHash(v[i], 10)%64;
    }

    num_of_collisions = 0;
    for (auto N = 10; N < std::size(hc_1); N+=10)
    {
        for(i = 0; i < N; ++i)
        {
            for (auto j = i+1; j < N; ++j)
            {
                if(hc_1[i] == hc_1[j])
                {
                    hc.insert(hc_1[i]);
                }
        
            }
        }
        num_of_collisions = N - std::size(hc);
        fout << N << "      " << num_of_collisions << std::endl;
        hc.clear();
    }

    
    
    fout << "data for PJWHash" << std::endl;

    for(auto i = 0; i < std::size(v); ++i)
    {
        hc_1[i] = PJWHash(v[i], 10)%64;
    }

    num_of_collisions = 0;
    for (auto N = 10; N < std::size(hc_1); N+=10)
    {
        for(i = 0; i < N; ++i)
        {
            for (auto j = i+1; j < N; ++j)
            {
                if(hc_1[i] == hc_1[j])
                {
                    hc.insert(hc_1[i]);
                }
        
            }
        }
        num_of_collisions = N - std::size(hc);
        fout << N << "      " << num_of_collisions << std::endl;
        hc.clear();
    }
   
   

    fout << "data for ELFHash" << std::endl;

    for(auto i = 0; i < std::size(v); ++i)
    {
        hc_1[i] = ELFHash(v[i], 10)%64;
    }

    num_of_collisions = 0;
    for (auto N = 10; N < std::size(hc_1); N+=10)
    {
        for(i = 0; i < N; ++i)
        {
            for (auto j = i+1; j < N; ++j)
            {
                if(hc_1[i] == hc_1[j])
                {
                    hc.insert(hc_1[i]);
                }
        
            }
        }
        num_of_collisions = N - std::size(hc);
        fout << N << "      " << num_of_collisions << std::endl;
        hc.clear();
    }

       
       
    fout << "data for BKDRHash" << std::endl;

    for(auto i = 0; i < std::size(v); ++i)
    {
        hc_1[i] = BKDRHash(v[i], 10)%64;
    }

    num_of_collisions = 0;
    for (auto N = 10; N < std::size(hc_1); N+=10)
    {
        for(i = 0; i < N; ++i)
        {
            for (auto j = i+1; j < N; ++j)
            {
                if(hc_1[i] == hc_1[j])
                {
                    hc.insert(hc_1[i]);
                }
        
            }
        }
        num_of_collisions = N - std::size(hc);
        fout << N << "      " << num_of_collisions << std::endl;
        hc.clear();
    }

       
       
    fout << "data for SDBMHash" << std::endl;

    for(auto i = 0; i < std::size(v); ++i)
    {
        hc_1[i] = SDBMHash(v[i], 10)%64;
    }

    num_of_collisions = 0;
    for (auto N = 10; N < std::size(hc_1); N+=10)
    {
        for(i = 0; i < N; ++i)
        {
            for (auto j = i+1; j < N; ++j)
            {
                if(hc_1[i] == hc_1[j])
                {
                    hc.insert(hc_1[i]);
                }
        
            }
        }
        num_of_collisions = N - std::size(hc);
        fout << N << "      " << num_of_collisions << std::endl;
        hc.clear();
    }
       
       
    fout << "data for DJBHash" << std::endl;

    for(auto i = 0; i < std::size(v); ++i)
    {
        hc_1[i] = DJBHash(v[i], 10) % 64;
    }

    num_of_collisions = 0;
    for (auto N = 10; N < std::size(hc_1); N+=10)
    {
        for(i = 0; i < N; ++i)
        {
            for (auto j = i+1; j < N; ++j)
            {
                if(hc_1[i] == hc_1[j])
                {
                    hc.insert(hc_1[i]);
                }
        
            }
        }
        num_of_collisions = N - std::size(hc);
        fout << N << "      " << num_of_collisions << std::endl;
        hc.clear();
    }
       
       
    fout << "data for DEKHash" << std::endl;

    for(auto i = 0; i < std::size(v); ++i)
    {
        hc_1[i] = DEKHash(v[i], 10)%64;
    }

    num_of_collisions = 0;
    for (auto N = 10; N < std::size(hc_1); N+=10)
    {
        for(i = 0; i < N; ++i)
        {
            for (auto j = i+1; j < N; ++j)
            {
                if(hc_1[i] == hc_1[j])
                {
                    hc.insert(hc_1[i]);
                }
        
            }
        }
        num_of_collisions = N - std::size(hc);
        fout << N << "      " << num_of_collisions << std::endl;
        hc.clear();
    }


    return 0;
}
