/**
 * @file    Options.cpp
 * @author  Marvin Smith
 * @date    2/15/2016
*/
#include "Options.hpp"


// C++ Libraries
#include <deque>
#include <iostream>
#include <string>

/********************************/
/*          Constructor         */
/********************************/
Options::Options( int argc, char* argv[] )
{

    // Parse the Command-Line
    Parse_Command_Line( argc, argv );

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





    }

}


