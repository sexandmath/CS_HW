#include <algorithm>
#include <future>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <thread>
#include <random>
#include <vector>
#include <map>
#include "Timer.h"



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

double myRandom()
{
    std::mt19937 engine_mt;
    std::uniform_real_distribution<double> myrandom(0, 1);
    return myrandom(engine_mt);
}



std::size_t Monte_Carlo(std::size_t N)
{
    std::size_t m = 0;

    for(auto i = 0; i < N; ++i)
    {
        if(std::pow(myRandom(), 2) + std::pow(myRandom(), 2) <= 1)
        {
            ++m;
        }
    }

    return m;
}

double consistent_Monte_Carlo(std::size_t N)
{
    return static_cast<double>(Monte_Carlo(N)*4)/N;
}


double parallel_Monte_Carlo(const std::size_t N)
{
    

    if (!N)
        return 0;

    const std::size_t min_per_thread = 25;
    const std::size_t max_threads =
        (N + min_per_thread - 1) / min_per_thread;

    const std::size_t hardware_threads =
        std::thread::hardware_concurrency();

    const std::size_t num_threads =
        std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

    const std::size_t block_size = N/ num_threads;

    std::vector < std::future < std::size_t > > futures(num_threads - 1);
    std::vector < std::thread >          threads(num_threads - 1);

    Threads_Guard guard(threads);

    

    for (std::size_t i = 0; i < (num_threads - 1); ++i)
    {
        std::packaged_task < std::size_t(std::size_t) > task(Monte_Carlo);

        futures[i] = task.get_future();
        threads[i] = std::thread(std::move(task), block_size);
    }

    std::size_t last_result = Monte_Carlo(block_size);

    std::size_t result = 0;

    for (std::size_t i = 0; i < (num_threads - 1); ++i)
    {
        result += futures[i].get();
    }

    result += last_result;

    return static_cast<double>(4*result)/N;
}



//parallel_Monte_Carlo faster then consistent
    



int main()
{
    {
    Timer t_cons("consistent_Monte_Carlo");
    std::cout << std::setprecision(10) << consistent_Monte_Carlo(10000000) << std::endl;
    }
    {
    Timer t_parallel("parallel_Monte_Carlo");
    std::cout << std::setprecision(10) << parallel_Monte_Carlo(10000000) << std::endl;
    }
    return 0;
}
