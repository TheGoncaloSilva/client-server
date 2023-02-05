// header of standard or provided libraries
#include <iostream>

// headers of local libraries
#include "client.h"

// Namespaces in use
using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

/* Default constructor*/
Client::Client(const string ip, const uint16_t port) noexcept
    : ip{ip},
      port{port},
      sAddress{address::from_string(ip), port},
      mSocket{ioService}
{
    cout << "Creating client" << endl;
}

/* Default destructor */
Client::~Client() noexcept
{
    disconnect_client();
}

bool Client::connect_client()
{
    cout << "Connecting" << endl;
    try{
        boost::system::error_code err;
        mSocket.connect(sAddress, err);
        if(err){
            cout << "Error code" << err.message() << endl;
            return false;
        }
    }catch(const std::exception &exception){
        cout << "Exception: " << exception.what() << endl;
        return false;
    }

    return true;
}

void Client::disconnect_client(){
    if(mSocket.is_open())
    {
        mSocket.close();
    }
}

void Client::client_life()
{
    if(!connect_client()){
        cout << "Problem connecting to server" << endl;
        return;
    }

    // Send a request to the server
    string request = "Hello Server!";
    if(write(mSocket, buffer(request, request.size())) != request.size()){
        cout << "The bytes weren't sent to the server" << endl;
    }

    // Receive the response from the server
    auto buffer = make_shared<vector<char>>(1024);
    mSocket.async_read_some(boost::asio::buffer(buffer->data(), buffer->size()),
                            [buffer, this](const boost::system::error_code& ec,
                                    size_t bytes_transferred) {
                                        handle_response(ec, bytes_transferred, buffer);
                                    }
                            );

    // Start the event loop
    ioService.run();
}

void Client::handle_response(const boost::system::error_code& ec,
                    size_t bytes_transferred,
                    shared_ptr<vector<char>> buffer)
{
    if (!ec) {
        cout << "Response received: ";
        cout.write(buffer->data(), bytes_transferred);
        cout << endl;
    } else {
        cout << "Error: " << ec.message() << endl;
    }
}

