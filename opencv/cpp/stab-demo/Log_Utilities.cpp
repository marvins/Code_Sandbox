/**
 * @file    Log_Utilities.cpp
 * @author  Marvin Smith
 * @date    1/22/2019
 */
#include "Log_Utilities.hpp"

// C++ Libraries
#include <iostream>
#include <mutex>
#include <sstream>

static std::mutex g_log_mtx;

/****************************************************/
/*          Convert Log Severity to String          */
/****************************************************/
std::string LogSeverityToString( LogSeverity severity )
{
    switch( severity )
    {
        case LogSeverity::ERROR:
            return "ERROR";
        case LogSeverity::WARNING:
            return "WARNING";
        case LogSeverity::INFO:
            return "INFO";
        case LogSeverity::DEBUG:
            return "DEBUG";
        case LogSeverity::TRACE:
            return "TRACE";
        case LogSeverity::UNKNOWN:
        default:
            return "UNKNOWN";
    }
    return "UNKNOWN";
}


/************************************/
/*          Log a Message           */
/************************************/
void Log_Message( LogSeverity  severity,
                  std::string  message )
{
    std::stringstream ss;
    ss << LogSeverityToString(severity) << "  " << message;

    g_log_mtx.lock();
    std::cout << ss.str() << std::endl;
    g_log_mtx.unlock();
}