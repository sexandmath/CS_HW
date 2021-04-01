#include <iostream>
#include <algorithm>
#include <vector>
#include <mutex>
#include <thread>
#include <fstream>
#include <string>
#include<future>
#include <iterator>


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

std::string readFile(std::ifstream& fin)
{
    return std::string((std::istreambuf_iterator<char>(fin)),
        std::istreambuf_iterator<char>());
}

std::mutex mut;
template<typename Iterator>
void matching(Iterator first, Iterator last, std::size_t len, std::string & d, std::vector<std::size_t> & v)
{
    for(auto i = first; i < last; ++i)
    {
        if (*i == d)
        {
            auto l = len + std::distance(first, i);
            std::lock_guard<std::mutex> lg(mut);
            v.push_back(l);
        }
    }
}

template<typename Iterator>
void parallel_find(Iterator beg, Iterator end, std::string data)
{
    const std::size_t length = std::distance(beg, end);
    std::vector<std::size_t> coincidences;

    if (!length)
        std::cout << "Something wrong" << '\n';
    
    const std::size_t num_threads =
    (std::thread::hardware_concurrency() != 0 ? std::thread::hardware_concurrency() : 2);

    

    const std::size_t block_size = length/ num_threads;

    std::vector < std::future < void > > futures(num_threads - 1);
    std::vector < std::thread >          threads(num_threads - 1);

    Threads_Guard guard(threads);

    Iterator first_block = beg;

    for (std::size_t i = 0; i < (num_threads - 1); ++i)
    {
        Iterator last_block = first_block;
        std::advance(last_block, block_size);

        std::packaged_task < void(Iterator, Iterator, std::size_t, std::string &, std::vector<std::size_t>&) > task(matching<Iterator>);

        futures[i] = task.get_future();
        threads[i] = std::thread(std::move(task), first_block, last_block, std::distance(beg, first_block), std::ref(data), std::ref(coincidences));

        first_block = last_block;
    }

    matching(first_block, end, std::distance(beg, first_block), std::ref(data), std::ref(coincidences));



    for (std::size_t i = 0; i < (num_threads - 1); ++i)
    {
        futures[i].get();
    }

    if(coincidences.empty())
    {
        std::cout << "No matches found" << std::endl;
    }
    else
    {
        std::for_each(std::begin(coincidences), std::end(coincidences), [](auto & elem){ std::cout << elem << " ";});
    }
}

int main()
{
    std::ifstream fin("hw8e2.txt");
    std::string data = readFile(fin);


    std::string data_to_find;
    std::cout << "Enter sequence to find: ";
    std::cin >> data_to_find;
    std::cout << std::endl;
    
    std::vector<std::string> seq(std::size(data)-std::size(data_to_find));
    std::size_t k = 0;

    for (auto i = std::begin(data); i < std::end(data); ++i)
    {
        if(*i == '\t' | *i == '\n' | *i == '\r')
        {
            data.erase(i);
        }
        std::for_each(i, std::next(i, std::size(data_to_find)), [&seq, &k](auto elem){ seq[k] += elem;
         } );
         ++k;
    }
    std::cout << std::size(data) << std::endl;
    parallel_find(std::begin(seq), std::end(seq), data_to_find);

    return 0;
}
