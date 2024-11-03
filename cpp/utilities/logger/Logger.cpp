#include "Logger.hpp"

#include <fstream>

using namespace std;

/**************************************************/
/*               Color Output Class               */
/**************************************************/
LogColor::LogColor( int v ): val(v){ }

ostream& operator<<( ostream& os, const LogColor& mp) 
{
   if(mp.val >= 0){
      os << "\033[0;" << mp.val << "m";
      return os;
   }
   else{
      os << "\033[1;" << fabs(mp.val) << "m";
      return os;
   }
}

/**************************************************/
/*           Log Message Implementation           */
/**************************************************/
LogMessage::LogMessage(){
    level = LOG_INFO;
    message = "";
}

LogMessage::LogMessage( std::string const& _message ){
    level = LOG_INFO;
    message = _message;
}

LogMessage::LogMessage( std::string const& _message, const unsigned int& _level ){
    level   = _level;
    message = _message;
}

LogMessage::LogMessage( const unsigned int& _level, std::string const& _message ){
    level   = _level;
    message = _message;
}

/**
 * Output Stream Operator
*/
std::ostream& operator << ( std::ostream& ostr, LogMessage const& log_message ){
   
   //print the log header
   if( log_message.level == LOG_MAJOR )
       ostr << LogColor(RED) << "[  MAJOR  ]" << color_end;
   else if( log_message.level == LOG_MINOR )
       ostr << LogColor(RED) << "[  MINOR  ]" << color_end; 
   else if( log_message.level == LOG_WARNING )
       ostr << LogColor(YELLOW) << "[ WARNING ]" << color_end;
   else if( log_message.level == LOG_INFO )
       ostr << LogColor(WHITE) << "[  INFO   ]" << color_end;
   else 
       throw LogMessage( LOG_MAJOR, "ERROR: Unknown log message level in operator << ");

    //print the message
    ostr << " :  " << log_message.message;
    return ostr;

}


/*****************************************************/
/*              Logger Implementation                */
/*****************************************************/
/**
 * Set logger defaults
 */
void Logger::setDefaults(){
    
    m_logPathname = "log.log";

    m_logLevel = LOG_INFO;
    m_logMode  = LOG_MODE_CONSOLE;

}

/**
 * Default constructor.  
*/
Logger::Logger( ){
    setDefaults();
}

/**
 * Default Constructor
 */
Logger::Logger( const int& log_mode, const int& log_level, const string& log_name ){

    // set filename
    m_logPathname = log_name;

    // set log level
    m_logLevel = log_level;

    // set log mode
    m_logMode = log_mode;

}



/**
 * This function is just a wrapper for the actual function which requires
 * the log message object.
*/
void Logger::add_message( const unsigned int& level, string const& message ){     
    add_message( LogMessage( level, message ) );
}

/**
 * This function is just a wrapper for the actual function which requires
 * the log message object.
*/
void Logger::add_message( string const& message, const unsigned int& level ){
    add_message( LogMessage( level, message ) );
}

/** 
 * This is the primary interface by which to add messages to the log
*/
void Logger::add_message( LogMessage const& log_message ){

    // make sure the level is high
    if(log_message.level <= m_logLevel ){
    
        //add message to log output
        if(m_logMode  & LOG_MODE_LOGFILE  == LOG_MODE_LOGFILE  ){

            // open file and append
            ofstream fout;
            fout.open( m_logPathname.native().c_str(), std::fstream::app );
            fout << log_message << endl;
            fout.close();
        }

        //if the console priority is high enough, then print to screen
        if(m_logMode  & LOG_MODE_CONSOLE  == LOG_MODE_CONSOLE  ){

            // open file and append
            cout << log_message << endl;
        }
    }

}

/**
 * Set log level
 */
void Logger::setLoggingLevel( const int& logLevel ){
    m_logLevel = logLevel;
}

/**
 * Set log mode
 */
void Logger::setLoggingMode( const int& logMode ){
    m_logMode = logMode;
}

/**
 * Set log filename
 */
void Logger::setLogFilename( const std::string& logPathname ){

    m_logPathname = logPathname;
}


