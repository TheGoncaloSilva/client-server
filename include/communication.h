// Directive similar to include guards, but simpler. It signals the 
// preprocessor to include this source file only once in compilation.
// Design to fight circular imports and other issues
#pragma once

// header of standard or provided libraries
#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>

using namespace boost::asio::ip;

class Communication{
    public:
        std::vector<char>& receive_data(tcp::socket socket);

        void send_data(tcp::socket socket, std::vector<char> &data);


    private:
        static const uint16_t headerSize = 4;    // in Bytes

        void read_bytes(std::vector<char> &buff);
};