/**
 * @file    time-utility.cpp
 * @author  Marvin Smith
 * @date    10/19/2015
 */


// C++ Libraries
#include <iostream>


// Utility Libraries
#include "Options.hpp"
#include "Print_Times.hpp"


/**
 * @brief Main Driver.
 */
int main( int argc, char* argv[] )
{
    
    // Parse the Command-Line Options
    Options options(argc, argv);
    

    //  Check if the program mode is for printing the time
    if( options.Get_Program_Mode() == ProgramMode::PRINT_TIMES )
    {
        Print_Times(options);
    }

    // Return
    return 0;
}

