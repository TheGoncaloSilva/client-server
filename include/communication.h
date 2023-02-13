// Directive similar to include guards, but simpler. It signals the 
// preprocessor to include this source file only once in compilation.
// Design to fight circular imports and other issues
#pragma once

// header of standard or provided libraries
#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

class Communication{
    public:

        shared_ptr<vector<char>> static do_read_async(shared_ptr<ip::tcp::socket> socket);
        void static do_write_async(shared_ptr<ip::tcp::socket> socket, string data);

        shared_ptr<vector<char>> static do_read_sync(shared_ptr<ip::tcp::socket> socket);
        void static do_write_sync(shared_ptr<ip::tcp::socket> socket, string data);

        shared_ptr<std::vector<char>> static receive_data(shared_ptr<tcp::socket> socket);

        void static send_data(shared_ptr<tcp::socket> socket, string data);

    private:
        static const uint16_t headerSize = 4;    // in Bytes
};