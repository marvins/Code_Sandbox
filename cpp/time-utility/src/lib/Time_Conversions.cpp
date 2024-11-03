/**
 * @file    Time_Conversions.cpp
 * @author  Marvin Smith
 * @date    10/19/2015
*/
#include "Time_Conversions.hpp"

// Time-Utility Libraries
#include "Time_Constants.hpp"


// C++ Libraries
#include <chrono>

namespace TM{


/**************************************************/
/*         Compute the Current Posix Time         */
/**************************************************/
double Get_Current_Posix_Time()
{
    return std::time(NULL);
}

/*****************************************************/
/*          Convert Julian To Unix Timestamp         */
/*****************************************************/
double Convert_Julian_To_Posix( const double& julian_date )
{
    return (julian_date - JULIAN_DATE_UNIX_EPOCH) * SECONDS_PER_DAY;
}


/*******************************************/
/*         Convert Posix to Julian         */
/*******************************************/
double Convert_Posix_To_Julian( const double& unix_timestamp )
{
    return (unix_timestamp / SECONDS_PER_DAY) + JULIAN_DATE_UNIX_EPOCH;
}

} // End of TM Namespace


