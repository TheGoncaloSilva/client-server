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

Server::Server(const string ip, const uint16_t port) noexcept
    : ip{ip},
      port{port},
      sAddress{address::from_string(ip), port},
      acceptor(new ip::tcp::acceptor(ioService, sAddress)),
      socket(new ip::tcp::socket(ioService))
{
    cout << "Initiating server" << endl;
}

bool Server::create_server()
{
    start_accept(acceptor, socket);

    ioService.run();
    return true;
}

void Server::terminate_server()
{
    ioService.stop();
}

void Server::handle_request(const boost::system::error_code& ec, size_t bytes_transferred, shared_ptr<ip::tcp::socket> socket)
{
    if (!ec)
    {
        cout << "Received data from client: " << socket->remote_endpoint().address().to_string() << endl;

        string response = "Hello from server";
        async_write(*socket, buffer(response), [](const boost::system::error_code& ec, size_t bytes_transferred){});
    }
    else
    {
        cout << "Error occurred: " << ec.message() << endl;
    }
}

void Server::start_accept(shared_ptr<ip::tcp::acceptor> acceptor, shared_ptr<ip::tcp::socket> socket)
{
    acceptor->async_accept(*socket, [acceptor, socket](const boost::system::error_code& ec){
        if (!ec)
        {
            cout << "Accepted client: " << socket->remote_endpoint().address().to_string() << endl;
            shared_ptr<array<char, 128>> buf(new array<char, 128>);
            async_read(*socket, buffer(*buf), boost::bind(handle_request, boost::placeholders::_1, boost::placeholders::_2, socket));
        }
        else
        {
            cout << "Client not accepted because: " << ec.message() << endl;
        }
    });
}