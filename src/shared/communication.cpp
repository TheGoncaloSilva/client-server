
// headers of local libraries
#include "communication.h"
#include "logging.h"

shared_ptr<std::vector<char>> Communication::receive_data(shared_ptr<tcp::socket> socket)
{
    size_t val;

    // Write Header
    async_read(*socket, buffer(&val, headerSize), [](const boost::system::error_code& ec, size_t bytes_transferred){
            if(ec){
                BOOST_LOG_TRIVIAL(info) << "Failure Receiving " << bytes_transferred << " bytes, code: " << ec << "; Reason: " << ec.message();
                BOOST_THROW_EXCEPTION(runtime_error("Failure Receiving data"));
            }
    });

    auto data = make_shared<vector<char>>(val);

    // Write body
    async_read(*socket, buffer(data->data(), data->size()), [](const boost::system::error_code& ec, size_t bytes_transferred){
            if(ec){
                BOOST_LOG_TRIVIAL(info) << "Failure Receiving " << bytes_transferred << " bytes, code: " << ec << "; Reason: " << ec.message();
                BOOST_THROW_EXCEPTION(runtime_error("Failure Receiving data"));
            }
    });

    BOOST_LOG_TRIVIAL(info) << "Received data -> Size: " << data->size() << " | " << data->data();

    return data;
}

void Communication::send_data(shared_ptr<tcp::socket> socket, string data)
{
    BOOST_LOG_TRIVIAL(info) << "Sending data -> Size: " << data.size() << " | " << data.data();
    size_t val = data.size();

    // Write Header
    async_write(*socket, buffer(&val, headerSize), [](const boost::system::error_code& ec, size_t bytes_transferred){
            if(ec){
                BOOST_LOG_TRIVIAL(info) << "Failure sending " << bytes_transferred << " bytes, code: " << ec << "; Reason: " << ec.message();
                BOOST_THROW_EXCEPTION(runtime_error("Failure Sending data"));
            }
    });

    // Write body
    async_write(*socket, buffer(data.data(), data.size()), [](const boost::system::error_code& ec, size_t bytes_transferred){
            if(ec){
                BOOST_LOG_TRIVIAL(info) << "Failure sending " << bytes_transferred << " bytes, code: " << ec << "; Reason: " << ec.message();
                BOOST_THROW_EXCEPTION(runtime_error("Failure Sending data"));
            }
    });
}
