#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

#include <boost/asio.hpp>

class Chat_client
{
    private:
        using tcp_t = boost::asio::ip::tcp;
        boost::asio::io_service io;
    public:
    
        explicit Chat_client(const std::string & user_name, const std::string &  IP): m_user_name(user_name),
        m_endpoint(boost::asio::ip::address::from_string(IP), m_port), socket(io, m_endpoint.protocol())
        {}
        
        ~Chat_client() noexcept = default;
    public:

    void run()
    {
        auto reader = std::thread(&Chat_client::accept_message, this);
        send_message();
        reader.join();

    }
    
    private:
    
    void accept_message()
    {
        while(!m_flag)
        {
            std::cout << read_message(socket) << std::endl;
        }
    }

    void send_message()
    {

        // tcp_t::socket socket(io, m_endpoint.protocol());

        socket.connect(m_endpoint);

        write_message(socket);

    }

    

      std::string read_message(tcp_t::socket & socket)

    {
        boost::asio::streambuf buffer;

        boost::asio::read_until(socket, buffer, '\n');

        std::string message;

        std::istream input_stream(&buffer);
        std::getline(input_stream, message, '\n');
        if(message == "exit")
        {
            m_flag = true;
        }
        return message;
    }

    void write_message(tcp_t::socket & socket)
    {
        std::string data;

        while(true)
        {

            std::getline(std::cin, data);

            if (data == "exit")
            {
               break;
            }
            boost::asio::write(socket, boost::asio::buffer(m_user_name + ':' + data + '\n'));
        }
    }

   



    private:

    std::string m_user_name;
    std::size_t m_port = 6666;
    std::size_t m_size = 30;
    bool m_flag = false;
    tcp_t::endpoint m_endpoint;
    tcp_t::socket socket;
   
  
};

int main()
{
    std::string user_name;

    std::cout << "Enter your name: ";

    std::getline(std::cin, user_name);
    Chat_client(user_name, "127.0.0.1").run();
  

    return 0;
}
