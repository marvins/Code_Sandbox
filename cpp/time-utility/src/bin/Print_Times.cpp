/**
 * @file    Print_Times.cpp
 * @author  Marvin Smith
 * @date    10/19/2015
 */
#include "Print_Times.hpp"

// C++ Libraries
#include <iostream>

// Time Utility Libraries
#include <TimeUtilities.hpp>

/********************************************/
/*          Print the Current Times         */
/********************************************/
void Print_Times( Options const& options )
{
    // Load the Leap-Second Table
    TM::Leap_Second_Table leap_second_table( options.Get_Leap_Second_Pathname() );

    // Make sure it loaded correctly
    if( leap_second_table.Is_Valid() == false ){
        std::cerr << "Error: Unable to load the leap-second config." << std::endl;
        options.Usage();
        std::exit(-1);
    }


    // Print the Unix Timestamp 
    const uint64_t unix_timestamp = TM::Get_Current_Posix_Time();
    std::cout << std::fixed << std::endl;
    std::cout << "Unix Time (s) : " << unix_timestamp << std::endl;
    
    // Print the Julian Date
    std::cout << "Julian Date   : " << TM::Convert_Posix_To_Julian(unix_timestamp) << std::endl;
    

    // Print the GPS Time 
    
    
    std::cout << "Leap Seconds  : " << leap_second_table.Get_Leap_Seconds(unix_timestamp) << std::endl;
}

