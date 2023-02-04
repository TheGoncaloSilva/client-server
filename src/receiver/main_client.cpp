// header of standard or provided libraries
#include <iostream>
#include <string>

// headers of local libraries
#include "client.h"

// Namespaces in use
using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

// Macros declarations
constexpr int PORT = 8000; 

int main(int argc, char* argv[]) 
{
    io_service ios;

    // Connect to the server
    tcp::endpoint ep(address::from_string("127.0.0.1"), PORT);
    tcp::socket socket(ios);
    socket.connect(ep);

    // Send a request to the server
    string request = "Hello Server!";
    write(socket, buffer(request));

    // Receive the response from the server
    auto buffer = make_shared<vector<char>>(1024);
    socket.async_read_some(boost::asio::buffer(buffer->data(), buffer->size()),
                            [buffer](const boost::system::error_code& ec,
                                    size_t bytes_transferred) {
                            client::handle_response(ec, bytes_transferred, buffer);
                            });

    // Start the event loop
    ios.run();

    return 0;
}