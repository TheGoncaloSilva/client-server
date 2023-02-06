// header of standard or provided libraries
#include <iostream>
#include <filesystem> // C++17

// headers of local libraries
#include "logging.h"

// Namespaces in use
using namespace std;
namespace logging = boost::log;
namespace keywords = boost::log::keywords;

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
            logging::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");

            logging::add_file_log(
                keywords::severity = logging::trivial::debug,
                keywords::file_name = folderName + logFileName,
                keywords::auto_flush = true,
                keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] [%ProcessID%] [%LineID%] %Message%"
            );

            logging::add_console_log(
                std::cout, 
                keywords::format = "[%Severity%]: %Message%"),
                keywords::severity = logging::trivial::info
            ;

            /*
            logging::core::get()->set_filter
            (
                logging::trivial::severity >= logging::trivial::info
            );*/

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