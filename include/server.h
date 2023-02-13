// Directive similar to include guards, but simpler. It signals the 
// preprocessor to include this source file only once in compilation.
// Design to fight circular imports and other issues
#pragma once

// header of standard or provided libraries
#include <stdint.h>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

// headers of local libraries
#include "logging.h"
#include "seeds.h"

// Namespaces in use
using namespace std;
using namespace boost::asio;
using namespace boost::posix_time;

class Server{
    public:
        Server(const string ip, const uint16_t port) noexcept;
        ~Server() noexcept;

        bool create_server();
        void terminate_server();

        const string ip;
        const uint16_t port;
        Seeds seeds;

    private:

        void start_accept();

        void do_read();
        void do_write();


        io_context ioContext;
        ip::tcp::endpoint sAddress;
        shared_ptr<ip::tcp::acceptor> acceptor;
        shared_ptr<ip::tcp::socket> socket;
};