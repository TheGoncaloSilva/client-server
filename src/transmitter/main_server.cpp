// header of standard or provided libraries
#include <iostream>
#include <string>

// headers of local libraries
#include "server.h"

// Namespaces in use
using namespace std;
using namespace boost::asio;
using namespace boost::posix_time;

// Macros in use
constexpr int32_t PORT = 8000;

int main(int argc, char* argv[])
{
    io_service service;
    ip::tcp::endpoint endpoint(ip::tcp::v4(), PORT);

    shared_ptr<ip::tcp::acceptor> acceptor(new ip::tcp::acceptor(service, endpoint));
    shared_ptr<ip::tcp::socket> socket(new ip::tcp::socket(service));

    server::start_accept(acceptor, socket);

    service.run();

    return 0;
}
