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
    start_accept();

    ioContext.run();
    return true;
}

void Server::terminate_server()
{
    BOOST_LOG_TRIVIAL(info) << "Terminating server";
    ioContext.stop();
}

void Server::start_accept()
{
    acceptor->async_accept(*socket, [this](const boost::system::error_code& ec){
        if (!ec)
        {
            BOOST_LOG_TRIVIAL(info) << "Accepted client from " << socket->remote_endpoint().address().to_string() << endl;
            do_read();
        }
        else
        {
            BOOST_LOG_TRIVIAL(error) << "Client not accepted because: " << ec.message() << endl;
        }
    });
}

void Server::do_read()
{
    // Receive a request from the client and bind the response function
    shared_ptr<array<char, 1024>> buff(new array<char, 1024>);
    // Read body
    async_read(*socket, buffer(buff->data(), 13), [buff, this](const boost::system::error_code& ec, size_t bytes_transferred){
            if(!ec){
                BOOST_LOG_TRIVIAL(info) << "Received from client: " << buff->data() << " | Size: " << bytes_transferred;
                do_write();
            }else{
                BOOST_LOG_TRIVIAL(info) << "Failure Receiving " << bytes_transferred << " bytes, code: " << ec << "; Reason: " << ec.message();
                BOOST_THROW_EXCEPTION(runtime_error("Failure Receiving data"));
            }
    });
}

void Server::do_write()
{
    // Process and send a response to the client
    string response = "Hello from server";
    async_write(*socket, buffer(response), [response, this](const boost::system::error_code& ec, size_t bytes_transferred){
            if(!ec){
                BOOST_LOG_TRIVIAL(info) << "Sent to client: " << response;
                do_read();
            }else{
                BOOST_LOG_TRIVIAL(info) << "Failure sending " << bytes_transferred << " bytes to client, code: " << ec << "; Reason: " << ec.message();
            }
    });
} 