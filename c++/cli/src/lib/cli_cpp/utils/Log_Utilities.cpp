/**
 * @file    Log_Utilities.cpp
 * @author  Marvin Smith
 * @date    5/21/2015
 */
#include "Log_Utilities.hpp"

// Namespaces
namespace logging   = boost::log;
namespace src       = boost::log::sources;
namespace keywords  = boost::log::keywords;
namespace sinks     = boost::log::sinks;

using namespace logging::trivial;


namespace UTILS{



/****************************************************/
/*      Convert Logging String to Log Level         */
/****************************************************/
logging::trivial::severity_level String_To_Level( const std::string& str ){

    if( str == "trace" ){
        return trace;
    }
    if( str == "debug" ){
        return debug;
    }
    if( str == "info" ){
        return info;
    }
    if( str == "warning"){
        return warning;
    }
    if( str == "fatal" ){
        return fatal;
    }
    else
        return info;
}


/****************************************************/
/*              Initialize the Logger               */
/****************************************************/
void Initialize_Logger( const std::string& severity_string,
                        const std::string& log_path )
{

    // Convert the severity string to a level
    logging::trivial::severity_level default_level = String_To_Level(severity_string);

    // Set the severity
    logging::core::get()->set_filter(severity >= default_level);

    // Set the log path
    logging::add_file_log( log_path );

    //logging::add_common_attributes();

    // Log
    BOOST_LOG_TRIVIAL(info) << "Logging initialized.";

}


} // End of UTILS Namespace

