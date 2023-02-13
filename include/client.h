// Directive similar to include guards, but simpler. It signals the 
// preprocessor to include this source file only once in compilation.
// Design to fight circular imports and other issues
#pragma once

// header of standard or provided libraries
#include <stdint.h>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind/bind.hpp>
#include <string>

// headers of local libraries
#include "logging.h"
#include "seeds.h"
#include "communication.h"

// Namespaces in use
using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::posix_time;

class Client
{

    public:
        Client(const string ip, const uint16_t port) noexcept;

        ~Client() noexcept;

        void client_life(uint16_t timerSeconds);

        const string ip;
        const uint16_t port;
        Seeds seeds;
    private:
        bool connect_client();

        void disconnect_client();

        void static contact_server(const boost::system::error_code& ec,
                    boost::asio::steady_timer* timer, 
                    shared_ptr<tcp::socket> mSocket, 
                    uint16_t timerSeconds);
        
        io_context ioContext;
        tcp::endpoint sAddress;
        shared_ptr<tcp::socket> mSocket;




};