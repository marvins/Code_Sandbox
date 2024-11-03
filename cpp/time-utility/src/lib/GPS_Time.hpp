/**
 * @file    GPS_Time.hpp
 * @author  Marvin Smith
 * @date    10/19/2015
 */
#ifndef __TIME_UTILITIES_LIB_GPS_TIME_HPP__
#define __TIME_UTILITIES_LIB_GPS_TIME_HPP__

// C++ Libraries
#include <string>

namespace TM{

/**
 * @class GPS_Time
*/
class GPS_Time
{
    public:
        
        /**
         * @brief Constructor given current unix time.
        */
        GPS_Time();
        

        /**
         * @brief Constructor given a unix timestamp.
         *
         * @param[in] unix_timestamp
        */
        GPS_Time( const double& unix_timestamp );
        
    private:
        
        /// Class Name
        std::string m_class_name;
        
        /// GPS Week
        int m_gps_week;

        /// Leap Seconds
        int m_leap_seconds;

        /// GPS Seconds
        double m_gps_seconds;

}; // End of GPS_Time Class



} // End of TM Namespace

#endif
