// header of standard or provided libraries
#include <iostream>
#include <future>
#include <chrono>
#include <thread>

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
      seeds{CLIENT},
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

void Client::client_life(uint8_t timerSeconds)
{
    if(!connect_client()){
        BOOST_LOG_TRIVIAL(debug) << "Problem connecting to server. Client terminating";
        return;
    }

    // Async timer, to contact the server for informatiomn
    //deadline_timer timer(ioService, boost::posix_time::seconds(timerSeconds));
    //timer.async_wait(boost::bind(Client::contact_server, &mSocket));
    contact_server();
    /*
    while (1)
    {
        
        this_thread::sleep_for(std::chrono::seconds(timerSeconds));
        // Start the event loop
        
    }*/
    ioService.run();
    
    
}

void Client::contact_server(){
    // Send a request to the server
    string request = "Hello Server!";
    BOOST_LOG_TRIVIAL(info) << "Sending to server: " << request;
    if(write(mSocket, buffer(request, request.size())) != request.size()){
        BOOST_LOG_TRIVIAL(warning) << "None or not all data was sent to the server";
    }

    // Receive the response from the server
    auto buffer = make_shared<vector<char>>(1024);
    mSocket.async_read_some(boost::asio::buffer(buffer->data(), buffer->size()),
                            [buffer](const boost::system::error_code& ec,
                                    size_t bytes_transferred) {
                                        handle_response(ec, bytes_transferred, buffer);
                                    }
                            );
}

void Client::handle_response(const boost::system::error_code& ec,
                    size_t bytes_transferred,
                    shared_ptr<vector<char>> buffer)
{
    if (!ec) {
        BOOST_LOG_TRIVIAL(info) << "Received from server: " << buffer->data();
    } else {
        BOOST_LOG_TRIVIAL(info) << "Problem receiving data from server: " << ec.message();
    }
}

