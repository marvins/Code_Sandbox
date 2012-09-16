#include "Logger.hpp"

#include <fstream>

using namespace std;

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
       ostr << "[  MAJOR  ]";
   else if( log_message.level == LOG_MINOR )
       ostr << "[  MINOR  ]"; 
   else if( log_message.level == LOG_WARNING )
       ostr << "[ WARNING ]";
   else if( log_message.level == LOG_INFO )
       ostr << "[  INFO   ]";
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
 * Default constructor.  
*/
Logger::Logger( ){

    log_filename = "log.log";
    console_run_state = LOG_MAJOR;
    logfile_run_state = LOG_MAJOR;
    write_current = false;
}

Logger::Logger( const string& log_name ){

    log_filename = log_name;
    console_run_state = LOG_MAJOR;
    logfile_run_state = LOG_MAJOR;
    write_current = false;
}

Logger::Logger( string const& log_file, const unsigned int& general_run_state ){

    log_filename = log_file;
    console_run_state = general_run_state;
    logfile_run_state = general_run_state;
    write_current = false;
}

/** 
 * Full Parameterized Constructor
*/
Logger::Logger( string const& log_file, const unsigned int& file_run_state, const unsigned int& print_run_state ){

    log_filename = log_file;
    console_run_state = print_run_state;
    logfile_run_state = file_run_state;
    write_current = false;
}

/**
 * Destructor 
 *
 * Makes sure that the log file has been written if required
*/
Logger::~Logger(){
    
    //check if current
    if( write_current == true )return;

    //make sure we can even write log messages
    if( logfile_run_state == LOG_NONE ) return;

    //otherwise write log messages we are allowed to write
    write_log();

}

unsigned int Logger::get_logfile_run_state() const{
    return logfile_run_state;
}

void Logger::set_logfile_run_state( const unsigned int& _logfile_run_state ){
    logfile_run_state = _logfile_run_state;
}

unsigned int Logger::get_console_run_state() const{
    return console_run_state;
}

void Logger::set_console_run_state( const unsigned int& _console_run_state ){
    console_run_state = _console_run_state;
}
       
void Logger::set_logfile_name( std::string const& filename ){
    log_filename = filename;
}

string Logger::get_logfile_name()const{
    return log_filename;
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
    
    //add message to log output
    if(  ( log_message.level & logfile_run_state ) == log_message.level )
        log_messages.push_back(log_message);

    //if the console priority is high enough, then print to screen
    if(  ( log_message.level & console_run_state ) == log_message.level )
        cout << log_message << endl;
    
    //make sure the write_current flag is false
    write_current = false;

}

void Logger::write_log(){
    
    //don't write if the log is current
    if( write_current == true )
        return;

    //create an output file
    ofstream fout;
    fout.open( log_filename.c_str());
    
    //output each log file
    deque<LogMessage>::iterator  it;
    for( it=log_messages.begin(); it != log_messages.end(); it++)
        fout << *it << endl;
    
    //close up shop
    fout.close();
    
    //make sure to mark the write_current flag as true
    write_current = true;

}
