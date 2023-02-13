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
            handle_session(socket);
            //do_read(socket);
        }
        else
        {
            BOOST_LOG_TRIVIAL(error) << "Client not accepted because: " << ec.message() << endl;
        }
    });
}

void Server::handle_session(shared_ptr<ip::tcp::socket> socket)
{
    // Read
    auto buff = Communication::do_read_sync(socket);

    BOOST_LOG_TRIVIAL(info) << "Received from client: " << buff->data() << " | Size: " << buff->size();

    // Process
    string response = "Hello from server";

    // Write
    //comms.send_data();
    Communication::do_write_sync(socket, response);
    
    BOOST_LOG_TRIVIAL(info) << "Sent to client: " << response;

    // Recursive call
    handle_session(socket);
}