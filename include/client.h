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

// Namespaces in use
using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

class Client
{

    public:
        Client(const string ip, const uint16_t port) noexcept;

        ~Client() noexcept;

        void client_life(uint8_t timerSeconds);

        const string ip;
        const uint16_t port;
        Seeds seeds;
    private:
        bool connect_client();

        void disconnect_client();

        void contact_server();

        void static handle_response(const boost::system::error_code& ec,
                        size_t bytes_transferred,
                        shared_ptr<vector<char>> buffer);

        io_service ioService;
        tcp::endpoint sAddress;
        tcp::socket mSocket;




};