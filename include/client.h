// Directive similar to include guards, but simpler. It signals the 
// preprocessor to include this source file only once in compilation.
// Design to fight circular imports and other issues
#pragma once

// header of standard or provided libraries
#include <stdint.h>
#include <boost/asio.hpp>
#include <string>

// Namespaces in use
using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

class Client
{

    public:
        Client(const string ip, const uint16_t port) noexcept;

        ~Client() noexcept;

        void client_life();

        const string ip;
        const uint16_t port;

    private:
        bool connect_client();

        void disconnect_client();

        void static handle_response(const boost::system::error_code& ec,
                        size_t bytes_transferred,
                        shared_ptr<vector<char>> buffer);

        io_service ioService;
        tcp::endpoint sAddress;
        tcp::socket mSocket;




};