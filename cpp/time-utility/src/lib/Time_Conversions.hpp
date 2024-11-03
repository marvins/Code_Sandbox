/**
 * @file    Time_Conversions.hpp
 * @author  Marvin Smith
 * @date    10/19/2015
*/
#ifndef __TIME_UTILITIES_LIB_TIME_CONVERSIONS_HPP__
#define __TIME_UTILITIES_LIB_TIME_CONVERSIONS_HPP__

namespace TM{

/**
 * @brief Get the Current Unix Timestamp
 *
 * @return Time in unix
*/
double Get_Current_Posix_Time();


/**
 * @brief Convert a Julian Date to a Unix Timestamp
 *
 * @param[in] julian_date
 *
 * @return Unix time.
*/
double Convert_Julian_To_Posix( const double& julian_date );


/**
 * @brief Convert a Unix Timestamp to Julian Date.
 *
 * @param[in] unix_timestamp
 *
 * @return Julian Date
*/
double Convert_Posix_To_Julian( const double& unix_timestamp );


} // End of TM Namespace


#endif
