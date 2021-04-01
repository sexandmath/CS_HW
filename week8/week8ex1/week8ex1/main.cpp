#include <algorithm>
#include <future>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <thread>
#include <random>
#include <vector>
#include <mutex>




class Threads_Guard
{
public:

    explicit Threads_Guard(std::vector < std::thread > & threads) :
        m_threads(threads)
    {}

    Threads_Guard            (Threads_Guard const&) = delete;

    Threads_Guard& operator=(Threads_Guard const&) = delete;

    ~Threads_Guard() noexcept
    {
        try
        {
            for (std::size_t i = 0; i < m_threads.size(); ++i)
            {
                if (m_threads[i].joinable())
                {
                    m_threads[i].join();
                }
            }
        }
        catch (...)
        {
            // std::abort();
        }
    }

private:

    std::vector < std::thread > & m_threads;
};


    
std::mutex mut;

std::size_t Monte_Carlo(std::size_t N, std::size_t & result)
{
    std::random_device rd;
    std::mt19937 engine_mt(rd());
    std::uniform_real_distribution<double> myrandom(0, 1);

    std::lock_guard<std::mutex> lg(mut);
       for(auto i = 0; i < N; ++i)
    {
           if(std::pow(myrandom(engine_mt), 2) + std::pow(myrandom(engine_mt), 2) <= 1)
           {
            ++result;
        }
    }
       return result;
}



double parallel_Monte_Carlo(const std::size_t N)
{
    if (!N)
    return 0;

    const std::size_t num_threads =
    (std::thread::hardware_concurrency() != 0 ? std::thread::hardware_concurrency() : 2);

    
    

    const std::size_t block_size = N/ num_threads;

    std::vector < std::future < std::size_t > > futures(num_threads - 1);
    std::vector < std::thread >          threads(num_threads - 1);

    Threads_Guard guard(threads);

    
    std::size_t result = 0;

    for (std::size_t i = 0; i < (num_threads - 1); ++i)
    {
        
        std::packaged_task < std::size_t(std::size_t, std::size_t &) > task(Monte_Carlo);

        futures[i] = task.get_future();
        threads[i] = std::thread(std::move(task), block_size, std::ref(result));
        
    }

    


    for (std::size_t i = 0; i < (num_threads - 1); ++i)
    {
         futures[i].get();
     }
    
    Monte_Carlo(block_size, std::ref(result));
    
    return 4.0*result/N;
    }
       



int main()
{
    
    std::cout << std::setprecision(10) << parallel_Monte_Carlo(10000000) << std::endl;
    
    return 0;
}
