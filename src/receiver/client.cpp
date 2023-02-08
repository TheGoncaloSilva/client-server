// header of standard or provided libraries
#include <iostream>
#include <future>
#include <chrono>

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
      mSocket(new ip::tcp::socket(ioContext))
{
    BOOST_LOG_TRIVIAL(trace) << "Creating the client class";
}

/* Default destructor */
Client::~Client() noexcept
{
    BOOST_LOG_TRIVIAL(trace) << "Destructing the client class";
    disconnect_client();
    ioContext.stop();
}

bool Client::connect_client()
{
    BOOST_LOG_TRIVIAL(info) << "Connecting to the server address " + ip + ":" + to_string(port);
    try{
        boost::system::error_code err;
        mSocket->connect(sAddress, err);
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
    if(mSocket->is_open())
    {
        BOOST_LOG_TRIVIAL(debug) << "Client connection terminated"; 
        mSocket->close();
    }
    else
    {
        BOOST_LOG_TRIVIAL(debug) << "Client was already disconnected";
    }
}

void Client::client_life(uint16_t timerSeconds)
{
    if(!connect_client()){
        BOOST_LOG_TRIVIAL(debug) << "Problem connecting to server. Client terminating";
        return;
    }

    // Async timer, to contact the server for informatiomn
    boost::asio::steady_timer timer(ioContext, boost::asio::chrono::seconds(timerSeconds));
    timer.async_wait(boost::bind(Client::contact_server, 
                            boost::asio::placeholders::error, 
                            &timer, 
                            mSocket,
                            timerSeconds
                        ));

    ioContext.run();
}

void Client::contact_server(const boost::system::error_code& ec,
                    boost::asio::steady_timer* timer, 
                    shared_ptr<tcp::socket> mSocket, 
                    uint16_t timerSeconds){
    if(!ec){

        // Send a request to the server
        string request = "Hello Server!";
        BOOST_LOG_TRIVIAL(info) << "Sending to server: " << request;
        async_write(*mSocket, buffer(request, request.size()), [](const boost::system::error_code& ec, size_t bytes_transferred){
                if(ec){
                    BOOST_LOG_TRIVIAL(info) << "Failure sending " << bytes_transferred << " bytes to server, code: " << ec << "; Reason: " << ec.message();
                }
            });

        // Receive the response from the server
        auto buff = make_shared<vector<char>>(1024);
        /*mSocket->async_read_some(boost::asio::buffer(buff->data(), buff->size()),
                                [buff](const boost::system::error_code& ec,
                                        size_t bytes_transferred) {
                                            handle_response(ec, bytes_transferred, buff);
                                        }
                                );*/
        async_read(*mSocket, buffer(buff->data(), 17), boost::bind(handle_response, boost::placeholders::_1, boost::placeholders::_2, buff));

        // Set up timers for continuation of client calls
        timer->expires_at(timer->expiry() + boost::asio::chrono::seconds(timerSeconds));
        timer->async_wait(boost::bind(Client::contact_server, 
                                boost::asio::placeholders::error, 
                                timer, 
                                mSocket,
                                timerSeconds));
    }else{
        BOOST_LOG_TRIVIAL(info) << "Failure contacting server, code: " << ec << "; Reason: " << ec.message();
    }
}

void Client::handle_response(const boost::system::error_code& ec,
                    size_t bytes_transferred,
                    shared_ptr<vector<char>> buffer)
{
    if (!ec) {
        BOOST_LOG_TRIVIAL(info) << "Received from server: " << buffer->data() << " | Size: " << bytes_transferred;
    } else {
        BOOST_LOG_TRIVIAL(info) << "Problem receiving data from server: " << ec.message();
    }
}

