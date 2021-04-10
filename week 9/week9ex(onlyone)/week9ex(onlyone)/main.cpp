#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <future>

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

namespace bint = boost::interprocess;

void writing(bint::managed_shared_memory & msh)
{
    using allocator_t = bint::allocator <std::pair<std::size_t, std::string>, bint::managed_shared_memory::segment_manager>;
    using vector_t = bint::vector <std::pair<std::size_t, std::string>, allocator_t>;


    auto v = msh.find <vector_t>("v").first;
    auto count = msh.find<std::size_t>("Counter").first;
    std::size_t coun = *count;
    auto mut = msh.find<bint::interprocess_mutex>("mut").first;
    auto cond = msh.find<bint::interprocess_condition>("cond").first;

    std::string text;

    do
    {
        std::cin >> text;
        std::pair<std::size_t, std::string> x = std::make_pair(coun, text);
        
        bint::scoped_lock locking(*mut);

        v->push_back(x);

        cond->notify_one();

    }  while (text != "exit");
    --(*count);
}

void reading(bint::managed_shared_memory & msh)
{
    using allocator_t = bint::allocator <std::pair<std::size_t, std::string>, bint::managed_shared_memory::segment_manager>;
    using vector_t = bint::vector <std::pair<std::size_t, std::string>, allocator_t>;


    auto v = msh.find <vector_t>("v").first;

    auto mut = msh.find<bint::interprocess_mutex>("mut").first;
    auto cond = msh.find<bint::interprocess_condition>("cond").first;

    std::string text;

    do
    {
        bint::scoped_lock locking(*mut);

        cond->wait(locking, [v]() {return !v->empty(); });

        std::pair<std::size_t, std::string> x = v->back();

        std::cout << x.first << ':' << x.second << std::endl;
        text = x.second;
        v->pop_back();
    }  while (text != "exit");
}


int main()
{
    bint::managed_shared_memory msh(bint::open_or_create, "noname", 1024);

    using allocator_t = bint::allocator <std::pair<std::size_t, std::string>, bint::managed_shared_memory::segment_manager>;
    using vector_t = bint::vector <std::pair<std::size_t, std::string>, allocator_t>;

    auto count = msh.find_or_construct<std::size_t>("Counter")(0);
    auto mut_counter = msh.find_or_construct<bint::interprocess_mutex>("mut_counter")();
    auto cond_counter = msh.find_or_construct<bint::interprocess_condition>("cond_counter")();
    auto mut = msh.find_or_construct<bint::interprocess_mutex>("mut")();
    auto cond = msh.find_or_construct<bint::interprocess_condition>("cond");

    
    ++(*count);
    cond_counter -> notify_one();
    

    auto v = msh.find_or_construct <vector_t>("v")(msh.get_segment_manager());

    std::thread uiu(writing, std::ref(msh));
    std::thread iu(reading, std::ref(msh));

    std::cout << "JOIN" << std::endl;
    iu.join();
    uiu.join();
   

    if (!(*count))
    {
        boost::interprocess::shared_memory_object::remove("noname");
    }
    return 0;
}
