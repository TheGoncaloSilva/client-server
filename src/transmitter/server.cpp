// header of standard or provided libraries
#include <iostream>

// headers of local libraries
#include "server.h"

// Namespaces in use
using namespace std;
using namespace boost::asio;
using namespace boost::posix_time;
using namespace boost::asio::ip;

// Macros declarations
constexpr int PORT = 1001; 

/* Default constructor*/
Server::Server(const string ip, const uint16_t port) noexcept
    : ip{ip},
      port{port},
      seeds{SERVER},
      sAddress{address::from_string(ip), port},
      acceptor(new ip::tcp::acceptor(ioContext, sAddress)),
      socket(new ip::tcp::socket(ioContext))
{
    BOOST_LOG_TRIVIAL(trace) << "Creating the server class";
}

/* Default destructor */
Server::~Server() noexcept
{
    BOOST_LOG_TRIVIAL(trace) << "Destructing the server class";
    terminate_server();
}

bool Server::create_server()
{
    BOOST_LOG_TRIVIAL(info) << "Setting up server with address " + ip + ":" + to_string(port);
    start_accept(acceptor, socket);

    ioContext.run();
    return true;
}

void Server::terminate_server()
{
    BOOST_LOG_TRIVIAL(info) << "Terminating server";
    ioContext.stop();
}

void Server::handle_request(const boost::system::error_code& ec, size_t bytes_transferred, shared_ptr<ip::tcp::socket> socket, shared_ptr<array<char, 1024>> buf)
{
    if (!ec)
    {
        BOOST_LOG_TRIVIAL(info) << "Received from client: " << buf->data() << " | Size: " << bytes_transferred;

        // Process and send a response to the client
        string response = "Hello from server";
        BOOST_LOG_TRIVIAL(info) << "Sending to client: " << response;
        async_write(*socket, buffer(response), [](const boost::system::error_code& ec, size_t bytes_transferred){
                if(ec){
                    BOOST_LOG_TRIVIAL(info) << "Failure sending " << bytes_transferred << " bytes to client, code: " << ec << "; Reason: " << ec.message();
                }
            });

        // Receive a request from the client and bind the response function
        shared_ptr<array<char, 1024>> buf(new array<char, 1024>);
        async_read(*socket, buffer(buf->data(), 13), boost::bind(handle_request, boost::placeholders::_1, boost::placeholders::_2, socket, buf));
    }
    else
    {
        BOOST_LOG_TRIVIAL(error) << "Problem receiving " << bytes_transferred << " bytes from client " << socket->remote_endpoint().address().to_string()
                                 << "; Reason: " << ec.value() << ", Message: " << ec.message();
    }
}

void Server::start_accept(shared_ptr<ip::tcp::acceptor> acceptor, shared_ptr<ip::tcp::socket> socket)
{
    acceptor->async_accept(*socket, [acceptor, socket](const boost::system::error_code& ec){
        if (!ec)
        {
            BOOST_LOG_TRIVIAL(info) << "Accepted client from " << socket->remote_endpoint().address().to_string() << endl;

            // Receive a request from the client and bind the response function
            shared_ptr<array<char, 1024>> buf(new array<char, 1024>);
            async_read(*socket, buffer(buf->data(), 13), boost::bind(handle_request, boost::placeholders::_1, boost::placeholders::_2, socket, buf));
        }
        else
        {
            BOOST_LOG_TRIVIAL(error) << "Client not accepted because: " << ec.message() << endl;
        }
    });
}