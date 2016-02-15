/**
 * @file    Options.cpp
 * @author  Marvin Smith
 * @date    2/14/2016
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
 : m_class_name("Options")
{
    // Parse the Command-Line
    Parse_Command_Line(argc, argv);
}


/***********************************************/
/*          Print Usage Instructions           */
/***********************************************/
void Options::Usage()
{
    // Print usage
    std::cerr << "usage: " << m_program_name << " [options]" << std::endl;
    std::cerr << std::endl;

    // Print Required Flags
    std::cerr << "required flags:" << std::endl;
    std::cerr << "-r <image-path> : Path to reference image." << std::endl;
    std::cerr << "-t <image-path> : Path to test image." << std::endl;
    std::cerr << std::endl;

}

/*******************************************/
/*         Parse the Command-Line          */
/*******************************************/
void Options::Parse_Command_Line( int argc, char* argv[] )
{
    // Set the Program Name
    m_program_name = argv[0];


    // Build a set of args
    std::deque<std::string> args;
    for( int i=1; i<argc; i++ ){
        args.push_back(argv[i]);
    }

    // Parse Command-Line Options
    std::string arg;
    while( args.size() > 0 )
    {
        // Get the next argument
        arg = args.front();
        args.pop_front();

        // Check if Help Requested
        if( arg == "-h" || arg == "--help" ){
            Usage();
            std::exit(-1);
        }

        // Check if Test Image Path
        else if( arg == "-t" ){
            m_test_image_path = args.front();
            args.pop_front();
        }

        // Check if Reference Image
        else if( arg == "-r" ){
            m_ref_image_path = args.front();
            args.pop_front();
        }

        // Otherwise, we have an error
        else
        {
            std::cerr << "Unknown argument: " << arg << std::endl;
            Usage();
            std::exit(-1);
        }

    }

}


