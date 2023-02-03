// header of standard or provided libraries
#include <stdint.h>
#include <boost/asio.hpp>

// Namespaces in use
using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

namespace client{

    void handle_response(const boost::system::error_code& ec,
                     size_t bytes_transferred,
                     shared_ptr<vector<char>> buffer);

}