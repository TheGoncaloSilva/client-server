// header of standard or provided libraries
#include <iostream>

// headers of local libraries
#include "server.h"

// Namespaces in use
using namespace std;
using namespace boost::asio;
using namespace boost::posix_time;

// Macros declarations
constexpr int PORT = 1001; 

namespace server{

    void handle_request(const boost::system::error_code& ec, size_t bytes_transferred, shared_ptr<ip::tcp::socket> socket)
    {
        if (!ec)
        {
            cout << "Received data from client: " << socket->remote_endpoint().address().to_string() << endl;

            string response = "Hello from server";
            async_write(*socket, buffer(response), [](const boost::system::error_code& ec, size_t bytes_transferred){});
        }
    }

    void start_accept(shared_ptr<ip::tcp::acceptor> acceptor, shared_ptr<ip::tcp::socket> socket)
    {
        acceptor->async_accept(*socket, [acceptor, socket](const boost::system::error_code& ec){
            if (!ec)
            {
                cout << "Accepted client: " << socket->remote_endpoint().address().to_string() << endl;
                shared_ptr<array<char, 128>> buf(new array<char, 128>);
                async_read(*socket, buffer(*buf), boost::bind(handle_request, boost::placeholders::_1, boost::placeholders::_2, socket));
                start_accept(acceptor, socket);
            }
        });
    }
}