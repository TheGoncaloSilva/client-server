// header of standard or provided libraries
#include <iostream>
#include <filesystem> // C++17

#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/core/null_deleter.hpp>
#include <fstream>

// headers of local libraries
#include "logging.h"

// Namespaces in use
using namespace std;
namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

// Macros
BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int);
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", logging::trivial::severity_level);
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", string);

namespace logger
{
    bool init_logging(const string folderName, const string logFileName) noexcept
    {
        try{
            set_folder_structure(folderName);
        }catch (const filesystem::filesystem_error& ex) {
            cerr << "Error creating logger directory: " << ex.what() << endl;
            return false;
        }

        try{
            
            // create a sink for file output, no filter
            logging::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");
            logging::add_file_log(
                keywords::severity = logging::trivial::debug,
                keywords::file_name = folderName + logFileName,
                keywords::auto_flush = true,
                keywords::filter = logging::trivial::severity >= logging::trivial::trace,
                keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] [%ProcessID%] [%LineID%] %Message%"
            );

            /*logging::add_console_log(
                std::cout, 
                keywords::format = "[%Severity%]: %Message%",
                keywords::filter = logging::trivial::severity >= logging::trivial::info
            );

            
            //logging::core::get()->set_filter
            //(
            //    logging::trivial::severity >= logging::trivial::info
            //);

            // Add attributes
            logging::add_common_attributes();*/

            // Setup the common formatter for all sinks
            logging::formatter fmt = expr::stream
                << std::setw(6) << std::setfill('0') << line_id << std::setfill(' ')
                << ": <" << severity << ">\t"
                << expr::if_(expr::has_attr(tag_attr))
                [
                    expr::stream << "[" << tag_attr << "] "
                ]
                << expr::smessage;
            
            typedef logging::sinks::synchronous_sink<logging::sinks::text_ostream_backend> TextSink;
            auto sink = boost::make_shared<TextSink>();
            
            /*
            // create a sink for file output, no filter
            sink->locked_backend()->add_stream(
                    boost::make_shared<std::ofstream>(folderName + logFileName));
            sink->set_formatter(fmt);
            logging::core::get()->add_sink(sink);*/

            // create another sink to std::cout, with a different filter
            boost::shared_ptr<std::ostream> stream(&std::cout, boost::null_deleter());
            sink = boost::make_shared<TextSink>();
            sink->locked_backend()->add_stream(stream);
            sink->set_filter(logging::trivial::severity >= logging::trivial::info);
            sink->set_formatter(
                expr::stream
                    << "[" << severity << "]: "
                    << expr::smessage
            );
            logging::core::get()->add_sink(sink);

            // Add attributes
            logging::add_common_attributes();



        }catch (const boost::exception& ex){
            cerr << "Error creating logger: " << boost::diagnostic_information(ex) << endl;
            return false;
        }catch (const std::exception& ex){
            cerr << "Error creating logger: " << boost::diagnostic_information(ex) << endl;
            return false;
        }
        
        return true;
    }

    void set_folder_structure(const string folder)
    {
        try{
            if(!filesystem::exists(folder)){
                filesystem::create_directories(folder);
            }
        }catch (const filesystem::filesystem_error& ex) { 
            throw;
        }
    }
};