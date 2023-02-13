
// headers of local libraries
#include "communication.h"
#include "logging.h"

shared_ptr<std::vector<char>> Communication::receive_data(shared_ptr<tcp::socket> socket)
{
    size_t val;

    // Read Header
    async_read(*socket, buffer(&val, headerSize), [](const boost::system::error_code& ec, size_t bytes_transferred){
            if(ec){
                BOOST_LOG_TRIVIAL(info) << "Failure Receiving " << bytes_transferred << " bytes, code: " << ec << "; Reason: " << ec.message();
                BOOST_THROW_EXCEPTION(runtime_error("Failure Receiving data"));
            }
    });

    auto data = make_shared<vector<char>>(val);

    // Read body
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




shared_ptr<vector<char>> Communication::do_read_async(shared_ptr<tcp::socket> socket)
{
    // Receive a request from the client and bind the response function
    auto buff = make_shared<vector<char>>(1024);
    // Read body
    async_read(*socket, buffer(buff->data(), 17), [buff](const boost::system::error_code& ec, size_t bytes_transferred){
            if(ec){
                BOOST_LOG_TRIVIAL(error) << "Failure Receiving " << bytes_transferred << " bytes, code: " << ec << "; Reason: " << ec.message();
                BOOST_THROW_EXCEPTION(runtime_error("Failure Receiving data"));
            }
    });

    return buff;
}

void Communication::do_write_async(shared_ptr<tcp::socket> socket, string data)
{
    // Process and send a response to the client
    async_write(*socket, buffer(data), [data](const boost::system::error_code& ec, size_t bytes_transferred){
            if(!ec){
                BOOST_LOG_TRIVIAL(info) << "Sent to Server: " << data;
            }else{
                BOOST_LOG_TRIVIAL(error) << "Failure sending " << bytes_transferred << " bytes to server, code: " << ec << "; Reason: " << ec.message();
            }
    });
} 

shared_ptr<vector<char>> Communication::do_read_sync(shared_ptr<ip::tcp::socket> socket)
{
    // Receive a request from the client and bind the response function
    auto buff = make_shared<vector<char>>(1024);
    // Read body
    if(read(*socket, buffer(buff->data(), 13)) <= 0){
        BOOST_LOG_TRIVIAL(error) << "Failure Receiving data from client";
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

void Communication::do_write_sync(shared_ptr<ip::tcp::socket> socket, string data)
{
    // Process and send a response to the client
    if(write(*socket, buffer(data)) != data.size()){
        BOOST_LOG_TRIVIAL(error) << "Failure sending data to client";
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
