/**
 * @file    Options.cpp
 * @author  Marvin Smith
 * @date    10/26/2015
*/
#include "Options.hpp"

// C++ Libraries
#include <deque>


/*********************************/
/*          Constructor          */
/*********************************/
Options::Options( int argc, char* argv[] )
  : m_class_name("Options")
{

    // Set the application name
    m_application_name = argv[0];


    // Set all arguments
    std::string arg;
    std::deque<std::string> args;
    for( int i=1; i<argc; i++ ){
        args.push_back(argv[i]);
    }


    // Parse remaining arguments
    while( args.size() > 0 )
    {
        // Grab the next argument
        arg = args.front();
        args.pop_front();
        
        // Check if help requested
        if( arg == "-h" ||
            arg == "--help" )
        {
            Usage();
            std::exit(-1);
        }

        // Check the base directory
        else if( arg == "-i" )
        {
            // Set the base directory
            m_base_directory = args.front();
            args.pop_front();
        }

        // Otherwise, failure
        else{
            std::cerr << "Unknown argument (" << arg << ")" << std::endl;
            Usage();
            std::exit(-1);
        }

    }

}


/****************************************/
/*      Print Usage Instructions        */
/****************************************/
void Options::Usage()
{
    // Print usage line
    std::cerr << "usage: " << m_application_name << " -i <base-directory>" << std::endl;
    std::cerr << std::endl;

}

