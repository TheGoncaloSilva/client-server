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
    BOOST_LOG_TRIVIAL(trace) << "Creating the client class";
}

/* Default destructor */
Client::~Client() noexcept
{
    BOOST_LOG_TRIVIAL(trace) << "Destructing the client class";
    disconnect_client();
}

bool Client::connect_client()
{
    BOOST_LOG_TRIVIAL(info) << "Connecting to the server address " + ip + ":" + to_string(port);
    try{
        boost::system::error_code err;
        mSocket.connect(sAddress, err);
        if(err){
            BOOST_LOG_TRIVIAL(fatal) << "Failure to connect to server. Reason: " << err.message();
            return false;
        }
    }catch(const std::exception &ex){
        BOOST_LOG_TRIVIAL(fatal) << "Failure to connect to server. Exception: " << ex.what();
        return false;
    }

    return true;
}

void Client::disconnect_client(){
    if(mSocket.is_open())
    {
        BOOST_LOG_TRIVIAL(debug) << "Client connection terminated"; 
        mSocket.close();
    }
    else
    {
        BOOST_LOG_TRIVIAL(debug) << "Client was already disconnected";
    }
}

void Client::client_life()
{
    if(!connect_client()){
        BOOST_LOG_TRIVIAL(debug) << "Problem connecting to server. Client terminating";
        return;
    }

    // Send a request to the server
    string request = "Hello Server!";
    BOOST_LOG_TRIVIAL(info) << "Sending to server: " << request;
    if(write(mSocket, buffer(request, request.size())) != request.size()){
        BOOST_LOG_TRIVIAL(warning) << "None or not all data was sent to the server";
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
        BOOST_LOG_TRIVIAL(info) << "Received from server: " << buffer;
        /*cout << "Response received: ";
        cout.write(buffer->data(), bytes_transferred);
        cout << endl;*/
    } else {
        BOOST_LOG_TRIVIAL(info) << "Problem receiving data from server: " << ec.message();
    }
}

