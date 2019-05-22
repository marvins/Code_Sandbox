/**
 * @file    Log_Utilities.hpp
 * @author  Marvin Smith
 * @date    1/22/2019
 */
#ifndef STAB_DEMO_LOG_UTILITIES_HPP
#define STAB_DEMO_LOG_UTILITIES_HPP

// C++ Libraries
#include <string>


enum class LogSeverity
{
    UNKNOWN,
    ERROR,
    WARNING,
    INFO,
    DEBUG,
    TRACE
};

std::string LogSeverityToString( LogSeverity severity );


void Log_Message( LogSeverity  severity,
                  std::string  message );

#define LOG_ERROR( message )   Log_Message( LogSeverity::ERROR,   message )
#define LOG_WARNING( message ) Log_Message( LogSeverity::WARNING, message )
#define LOG_INFO( message )    Log_Message( LogSeverity::INFO,    message )
#define LOG_DEBUG( message )   Log_Message( LogSeverity::DEBUG,   message )
#define LOG_TRACE( message )   Log_Message( LogSeverity::TRACE,   message )


#endif