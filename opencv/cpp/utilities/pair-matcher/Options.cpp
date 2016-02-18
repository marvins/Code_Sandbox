/**
 * @file    Options.cpp
 * @author  Marvin Smith
 * @date    2/15/2016
*/
#include "Options.hpp"


// Boost Libraries
#include <boost/algorithm/string.hpp>


// C++ Libraries
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>


/********************************/
/*          Constructor         */
/********************************/
Options::Options( int argc, char* argv[] )
{

    // Parse the Command-Line
    Parse_Command_Line( argc, argv );

    // Parse Config File
    Parse_Config_File();
}


/*********************************************/
/*          Print Usage Instructions         */
/*********************************************/
void Options::Usage()
{
    std::cerr << "usage: " << m_program_name << " [required flags] [optional flags]" << std::endl;

}


/************************************************/
/*          Parse Command-Line Options          */
/************************************************/
void Options::Parse_Command_Line( int argc, char* argv[] )
{
    // Set the program name
    m_program_name = argv[0];

    // Create list of args
    std::deque<std::string> args;
    std::string arg;
    for( int i=1; i<argc; i++ ){
        args.push_back(argv[i]);
    }


    // Process arguments
    while( args.size() > 0 )
    {

        // Grab the next argument
        arg = args.front();
        args.pop_front();

        // Check if Usage Requested
        if( arg == "-h" ||
            arg == "--help" )
        {
            Usage();
            std::exit(-1);
        }

        // Check if Config File
        else if( arg == "-c" ||
                 arg == "--config" )
        {
            m_config_pathname = args.front();
            args.pop_front();
        }


        // Check if Test File
        else
        {
            m_config_settings["TEST_IMAGE_PATH"] = arg;
        }


    }

}


/**************************************************/
/*          Parse the Configuration File          */
/**************************************************/
void Options::Parse_Config_File()
{

    // Open the file
    std::fstream fin;
    fin.open(m_config_pathname.c_str());

    
    // Grab the first line
    std::string line;
    fin >> line;
    std::string key, value;
    size_t pos;

    // Load while lines are available
    while( fin.good() )
    {
        // Trim whitespaces
        boost::trim_if( line, boost::is_any_of(" "));

        // Check the length or if it is a comment
        if( line.size() <= 0 || line[0] == '#' )
        {
            fin >> line;
            continue;
        }

        // Split the line
        pos = line.find("=");

        // Check for no value
        if( pos == std::string::npos ){
            fin >> line;
            continue;
        }

        // Trim and create key/value pair
        key   = boost::trim_copy_if( line.substr(0,pos), boost::is_any_of(" "));
        value = boost::trim_copy_if( line.substr(pos+1),   boost::is_any_of(" "));

        // Add to the Config
        m_config_settings[key] = value;

        // Get the next line
        fin >> line;
    }

    // Close the file
    fin.close();
    
}


