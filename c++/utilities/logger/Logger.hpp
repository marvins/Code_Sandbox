#ifndef __SRC_CORE_LOGGER_HPP__
#define __SRC_CORE_LOGGER_HPP__

#include <cmath>
#include <deque>
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>

/**
 * Keywords for the Log
 * - MAJOR:   Program-ending failures
 * - MINOR:   Possible Program-ending failures. More like invalid data being used.
 * - WARNING: A situation exists which may compromise program 
              success down the road.
 * - INFO:    Informational notes, program status, etc. 
*/
const unsigned int LOG_NONE = 0;
const unsigned int LOG_MAJOR=1;
const unsigned int LOG_MINOR=2;
const unsigned int LOG_WARNING=4;
const unsigned int LOG_INFO=8;
const unsigned int LOG_DEBUG=16;

/**
 * Logging Modes
 */
const unsigned int LOG_MODE_NONE     = 0;
const unsigned int LOG_MODE_CONSOLE  = 1;
const unsigned int LOG_MODE_LOGFILE  = 2;

/************************************************************************/
enum COLORS  { RED = 31, YELLOW = 33, GREEN = 32, BLUE = 34, CYAN = 36, WHITE = 37 };

#define color_end "\033[0m"

class LogColor
{
public:
    LogColor(int v);
    
    friend std::ostream& operator<<( std::ostream& os, const LogColor& mp);

private:
    int val;
};

/*************************************************************************/

/**
 * @class LogMessage
 * 
 * A class for passing and outputting logging messages.
*/
class LogMessage{
    public:
       
        /**
         * Default Constructor
        */
        LogMessage();
        
        /**
         * Parameterized Constructor
         *
         * @param[in] message Message to pass
        */
        LogMessage( std::string const& _message );
        
        /**
         * Parameterized Constructor
         *
         * @param[in] level   Priority of message
         * @param[in] message Message to pass
        */
        LogMessage( std::string const& _message, const unsigned int& _level );
        
        /**
         * Parameterized Constructor
         *
         * @param[in] level   Priority of message
         * @param[in] message Message to pass
        */
        LogMessage( const unsigned int& _level, std::string const& _message );
        
        unsigned int    level;   /*< Message Priority */
        std::string    message;  /*< Message Contents */
};

std::ostream& operator << ( std::ostream& ostr, LogMessage const& log_message );



class Logger{

    public:
        
        /** 
         * Default Constructor 
        */
        Logger();

        /**
         * Parameterized Constructor 
         * 
         * @param[in] log_file Name of file to output logging to
         *
        */
        Logger( const int& log_mode, const int& log_level, std::string const& log_file = "" );
        
        
        /** 
         * Add a message to the log
         *
         * @param[in] log_message
        */
        void add_message( LogMessage const& log_message );
        
        /** 
         * Add a message to the log
         * 
         * @param[in] level    message level
         * @param[in] Message  message
        */
        void add_message( const unsigned int& level, std::string const& message );
        
        /** 
         * Add a message to the log
         * 
         * @param[in] level    message level
         * @param[in] Message  message
        */
        void add_message( std::string const& message, const unsigned int& level );
        
        /**
         * Set the logging level
         *
         * @param[in] logLevel Logging level
         */
        void setLoggingLevel( const int& logLevel );

        /**
         * Set the logging mode
         *
         * @param[in] logMode Logging mode
         */
        void setLoggingMode( const int& logMode );
    
        /**
         * Set the log filename
         */
        void setLogFilename( const std::string& logPathname );

    private:

        /**
         * Set the default configuration for the logger
         */
        void setDefaults();


        boost::filesystem::path m_logPathname; /*< Name of log file */
        
        unsigned int m_logLevel; /*< Logging Level */
        unsigned int m_logMode;  /*< Logging Mode  */

};


#endif

