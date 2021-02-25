#pragma Once
#include <iostream>
#include <chrono>



class Timer
{
public:
    using clock_t = std::chrono::steady_clock;
    using time_point_t = clock_t::time_point;
    using duration_t = std::chrono::microseconds;
    Timer(const std::string &name = "Timer") : m_name(name), duration(0), m_start(clock_t::now()) {}

    void pause()
    {
         duration += std::chrono::duration_cast<duration_t>(clock_t::now() - m_start).count();
    }

    void contin()
    {
        m_start = clock_t::now();
    }

    std::string name()
    {
        return m_name;
    }

    long long drtn()
    {
        return duration;
    }

    ~Timer()
    {
        duration += std::chrono::duration_cast<duration_t>(clock_t::now() - m_start).count();
//        std::cout << m_name << std::endl;
//        std::cout << "Lead time(ms): " << duration << std::endl;
    }
private:
    time_point_t m_start;
    long long duration;
    std::string m_name;
    
};
