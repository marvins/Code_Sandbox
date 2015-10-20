/**
 * @file   Options.cpp
 * @author Marvin Smith
 * @date   10/19/2015
 */
#include "Options.hpp"


// C++ Libraries
#include <cstdlib>
#include <deque>
#include <iostream>


/**********************************/
/*          Constructor           */
/**********************************/
Options::Options( int argc, char* argv[] )
 : m_class_name("Options"),
   m_program_mode(ProgramMode::PRINT_TIMES),
   m_leap_second_pathname("data/leap-seconds.csv")
{

    // Parse the Command-Line
    Parse_Command_Line(argc, argv);

}


/************************************************/
/*          Print the Usage Instructions        */
/************************************************/
void Options::Usage()const
{
    // Print usage line
    std::cout << "usage: " << m_program_name << " [flags] " << std::endl;
    std::cout << std::endl;

    // Print Optional Arguments
    std::cout << "  Flags:" << std::endl;
    std::cout << "    -h|--help|--usage      Print usage and exit." << std::endl;
    std::cout << "    -l <leap second csv>   Override default leap-second CSV file path (Default=" << m_leap_second_pathname << ")." << std::endl;
    std::cout << std::endl;

}


/******************************************/
/*          Parse the command-line        */
/******************************************/
void Options::Parse_Command_Line( int argc, char* argv[] )
{
    // Set the program name
    m_program_name = argv[0];

    // Load remaining arguments into deque
    std::deque<std::string> args;
    for( int i=1; i<argc; i++ ){
        args.push_back(argv[i]);
    }


    // Parse remaining arguments
    std::string arg;
    while( args.size() > 0 )
    {
        // Grab the next argument
        arg = args.front();
        args.pop_front();


        // Check if help requested
        if( arg == "-h" || arg == "--help" || arg == "--usage" )
        {
            Usage();
            std::exit(0);
        }
        

        //  Check if leap-second file requested
        else if( arg == "-l" )
        {
            m_leap_second_pathname = args.front();
            args.pop_front();
        }

        // Otherwise, Unknown value
        else
        {
            std::cerr << "error: Unknown argument (" << arg << ")." << std::endl;
            Usage();
            std::exit(1);
        }

    }

}

