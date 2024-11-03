/**
 * @file    Leap_Second_Table.hpp
 * @author  Marvin Smith
 * @date    10/19/2015
*/
#ifndef __TIME_UTILITY_LIB_LEAP_SECOND_TABLE_HPP__
#define __TIME_UTILITY_LIB_LEAP_SECOND_TABLE_HPP__

// C++ Libraries
#include <string>
#include <tuple>
#include <vector>


namespace TM{

/**
 * @class Leap_Second_Table
*/
class Leap_Second_Table
{
    public:
        
        /**
         * @brief Constructor
         *
         * @param[in] pathname Path to Leap-Second data file.
        */
        Leap_Second_Table( const std::string& pathname );
        

        /**
         * @brief Check if table data is valid.
        */
        inline bool Is_Valid()const{
            return m_is_valid;
        }


        /**
         * @brief Get the number of leap seconds.
         *
         * @param[in] unix_timestamp
         *
         * @return Leap seconds.
        */
        double Get_Leap_Seconds( const double& unix_timestamp )const;


    private:
        
        /// Class Name
        std::string m_class_name;
        
        /// Flag if the config file loaded correctly
        bool m_is_valid;

        /// Mapping of Julian Dates to Unix Times
        std::vector<std::tuple<double,double>> m_posix_leap_second_table;

}; // End of Leap_Second_Table Class

} // End of TM Namespace

#endif
