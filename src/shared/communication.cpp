
// headers of local libraries
#include "communication.h"
#include "logging.h"

/* CLIENT */
shared_ptr<vector<char>> Communication::do_read_async(shared_ptr<tcp::socket> socket)
{   
    // Get Header
    int32_t val = 0;
    async_read(*socket, buffer(&val, sizeof(val)), [](const boost::system::error_code& ec, size_t bytes_transferred){
            if(ec){
                BOOST_LOG_TRIVIAL(error) << "Failure Receiving " << bytes_transferred << " bytes, code: " << ec << "; Reason: " << ec.message();
                BOOST_THROW_EXCEPTION(runtime_error("Failure Receiving data"));
            }
    });

    auto buff = make_shared<vector<char>>(val);
    // Read body
    async_read(*socket, buffer(buff->data(), val), [buff](const boost::system::error_code& ec, size_t bytes_transferred){
            if(ec){
                BOOST_LOG_TRIVIAL(error) << "Failure Receiving " << bytes_transferred << " bytes, code: " << ec << "; Reason: " << ec.message();
                BOOST_THROW_EXCEPTION(runtime_error("Failure Receiving data"));
            }
    });

    return buff;
}

/* CLIENT */
void Communication::do_write_async(shared_ptr<tcp::socket> socket, string data)
{   
    // Send message header
    int32_t val = data.size();
    async_write(*socket, buffer(&val, sizeof(val)), [val](const boost::system::error_code& ec, size_t bytes_transferred){
            if(!ec){
                BOOST_LOG_TRIVIAL(info) << "Sent to Server: " << val;
            }else{
                BOOST_LOG_TRIVIAL(error) << "Failure sending " << bytes_transferred << " bytes, code: " << ec << "; Reason: " << ec.message();
            }
    });

    // Send message body
    async_write(*socket, buffer(data), [data](const boost::system::error_code& ec, size_t bytes_transferred){
            if(!ec){
                BOOST_LOG_TRIVIAL(info) << "Sent to Server: " << data;
            }else{
                BOOST_LOG_TRIVIAL(error) << "Failure sending " << bytes_transferred << " bytes, code: " << ec << "; Reason: " << ec.message();
            }
    });
} 

/* SERVER */
shared_ptr<vector<char>> Communication::do_read_sync(shared_ptr<ip::tcp::socket> socket)
{
    // Get Header
    int32_t val = 0;
    if(read(*socket, buffer(&val, sizeof(val))) <= 0){
        BOOST_LOG_TRIVIAL(error) << "Failure Receiving data";
        BOOST_THROW_EXCEPTION(runtime_error("Failure Receiving data"));
    }

    // Receive a request from the client and bind the response function
    auto buff = make_shared<vector<char>>(val);
    // Read body
    if(read(*socket, buffer(buff->data(), val)) <= 0){
        BOOST_LOG_TRIVIAL(error) << "Failure Receiving data";
        BOOST_THROW_EXCEPTION(runtime_error("Failure Receiving data"));
    }
    
    /*async_read(*socket, buffer(buff->data(), 13), [buff, socket](const boost::system::error_code& ec, size_t bytes_transferred){
            if(!ec){
                BOOST_LOG_TRIVIAL(info) << "Received from client: " << buff->data() << " | Size: " << bytes_transferred;
                //do_write(socket);
            }else{
                BOOST_LOG_TRIVIAL(error) << "Failure Receiving " << bytes_transferred << " bytes, code: " << ec << "; Reason: " << ec.message();
                BOOST_THROW_EXCEPTION(runtime_error("Failure Receiving data"));
            }
    });*/

    return buff;
}

/* SERVER */
void Communication::do_write_sync(shared_ptr<ip::tcp::socket> socket, string data)
{   
    // Send message header
    int32_t val = data.size();
    // Process and send a response to the client
    if(write(*socket, buffer(&val, sizeof(val))) <= 0){
        BOOST_LOG_TRIVIAL(error) << "Failure sending header";
        BOOST_THROW_EXCEPTION(runtime_error("Failure Receiving data"));
    }

    // Process and send a response to the client
    if(write(*socket, buffer(data, data.size())) != data.size()){
        BOOST_LOG_TRIVIAL(error) << "Failure sending data";
        BOOST_THROW_EXCEPTION(runtime_error("Failure Receiving data"));
    }

    /*async_write(*socket, buffer(data), [data, socket](const boost::system::error_code& ec, size_t bytes_transferred){
            if(!ec){
                BOOST_LOG_TRIVIAL(info) << "Sent to client: " << data;
                //do_read(socket);
            }else{
                BOOST_LOG_TRIVIAL(error) << "Failure sending " << bytes_transferred << " bytes to client, code: " << ec << "; Reason: " << ec.message();
            }
    });*/
} 
