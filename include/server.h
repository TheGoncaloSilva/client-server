// header of standard or provided libraries
#include <stdint.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

// Namespaces in use
using namespace std;
using namespace boost::asio;
using namespace boost::posix_time;

namespace server{

    void handle_request(const boost::system::error_code& ec, 
                    size_t bytes_transferred, 
                    shared_ptr<boost::asio::ip::tcp::socket> socket);

    void start_accept(shared_ptr<boost::asio::ip::tcp::acceptor> acceptor, 
                    shared_ptr<boost::asio::ip::tcp::socket> socket);
}