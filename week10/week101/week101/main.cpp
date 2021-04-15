#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

#include <boost/asio.hpp>

class Chat_server
{
    private:
        using tcp_t = boost::asio::ip::tcp;
        boost::asio::io_service io;
    public:
        explicit Chat_server(const std::size_t port, const std::size_t size): m_port(port), m_size(size),
        m_endpoint(boost::asio::ip::address_v4::any(), m_port), socket(io){}
        
        
        ~Chat_server() noexcept = default;

    public:
    
    void run()
    {
        
        auto reader = std::thread(&Chat_server::accept_message, this);
         send_message();
        reader.join();

    }
    private:

    void accept_message()
    {
        tcp_t::acceptor acceptor(io, m_endpoint.protocol());

        acceptor.bind(m_endpoint);

        acceptor.listen(m_size);

        // boost::asio::ip::tcp::socket socket(io);

        acceptor.accept(socket);
        
        while(!m_flag)
        {
            std::cout << read_message(socket) << std::endl;
        }

        std::cout << "somebody exit" << std::endl;

    }

        void send_message()
    {

        // tcp_t::socket socket(io, m_endpoint.protocol());

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
    bool m_flag = false;
    std::string m_user_name = "Server";
    std::size_t m_port;
    std::size_t m_size;
    tcp_t::socket socket;
    tcp_t::endpoint m_endpoint;
    };

int main()
{
    // std::string user_name;

    // std::cout << "Enter your name: ";

    // std::getline(std::cin, user_name);
    
    Chat_server(6666, 30).run();

    return 0;
}
