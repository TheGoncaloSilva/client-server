// Directive similar to include guards, but simpler. It signals the 
// preprocessor to include this source file only once in compilation.
// Design to fight circular imports and other issues
#pragma once

#define BOOST_LOG_DYN_LINK 1

// header of standard or provided libraries
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/exception/diagnostic_information.hpp>

namespace logger
{
    bool init_logging(std::string folderName, std::string logFileName) noexcept;

    void set_folder_structure(std::string folder);
};