#ifndef __SRC_CORE_LOGGER_HPP__
#define __SRC_CORE_LOGGER_HPP__

#include <deque>
#include <iostream>
#include <string>

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
        Logger( std::string const& log_file );
        
        /**
         * Parameterized Constructor 
         * 
         * @param[in] log_file Name of file to output logging to
         * @param[in] general_run_state State to manage both file and console printing
         *
        */
        Logger( std::string const& log_file, const unsigned int& general_run_state );
        
        /**
         * Parameterized Constructor 
         * 
         * @param[in] log_file Name of file to output logging to
         * @param[in] file_run_state Which types of errors to print to file
         * @param[in] print_run_state Which types of errors to print to console
         *
        */
        Logger( std::string const& log_file, const unsigned int& file_run_state, const unsigned int& print_run_state );
        
        /** 
         * Destructor
         * 
         * If the log file has not been written or out of date, and it is desired, then it 
         * will be called here. 
        */
        ~Logger();

        /**
         * Retrieve the run condition for the console logging
         *
         * @return console_run_state value
        */
        unsigned int get_console_run_state() const;
        
        /** 
         * Set the run condition for the console logging
         * 
         * @param[in] _console_run_state Desired console run state
        */
        void set_console_run_state( const unsigned int& _console_run_state );

        /**
         * Retrieve the run condition for the file logging
         *
         * @return logfile_run_state value
        */
        unsigned int get_logfile_run_state() const;
    
        /** 
         * Set the run condition for the file logging
         * 
         * @param[in] _logfile_run_state Desired logfile run state
        */
        void set_logfile_run_state( const unsigned int& _logfile_run_state );

        /**
         * Set the logfile name
         * 
         * @param[in] filename 
        */
        void set_logfile_name( std::string const& filename );

        /**
         * Get the logfile name
         *
         * @return logfile name
        */
        std::string get_logfile_name()const;
        
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
    
    private:

        /**
         * Logger Writer
        */
        void write_log();

        std::string log_filename; /*< Name of log file */
        
        unsigned int logfile_run_state;   /*< Which type of errors to report to file */
        unsigned int console_run_state;   /*< Which type of errors to report to cout */
        
        std::deque<LogMessage> log_messages;  /*< List of log messages */

        bool write_current; /*< True if written data, and data is still currrent */

};


#endif

