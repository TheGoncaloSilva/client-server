// header of standard or provided libraries
#include <iostream>

// headers of local libraries
#include "client.h"

// Namespaces in use
using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

namespace client{

    void handle_response(const boost::system::error_code& ec,
                     size_t bytes_transferred,
                     shared_ptr<vector<char>> buffer)
    {
        if (!ec) {
            cout << "Response received: ";
            cout.write(buffer->data(), bytes_transferred);
            cout << endl;
        } else {
            cout << "Error: " << ec.message() << endl;
        }
    }

}