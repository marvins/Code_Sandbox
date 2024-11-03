/**
 * @file   Leap_Second_Table.cpp
 * @author Marvin Smith
 * @date   10/19/2015
*/
#include "Leap_Second_Table.hpp"

// C++ Libraries
#include <fstream>
#include <iostream>
#include <vector>


// Time Utility Libraries
#include "File_Utilities.hpp"
#include "Time_Conversions.hpp"

namespace TM{

/********************************/
/*          Constructor         */
/********************************/
Leap_Second_Table::Leap_Second_Table( const std::string& pathname )
  : m_class_name("Leap_Second_Table"),
    m_is_valid(false)
{
    // Julian
    double julian_date, unix_timestamp, leap_seconds;

    // Parse the CSV File
    std::vector<std::string> headers;
    std::vector<std::vector<std::string>> csv_data;
    csv_data = Parse_CSV_File( pathname, headers, true );
    
    // Load each row into fields
    for( size_t i=0; i<csv_data.size(); i++ )
    {
        // Convert Julian Date to Unix
        julian_date    = std::stod(csv_data[i][3]);
        unix_timestamp = Convert_Julian_To_Posix(julian_date); 

        // Get the leap seconds
        leap_seconds = std::stod(csv_data[i][4]);
        
        // Add to table
        m_posix_leap_second_table.emplace_back(std::make_tuple(unix_timestamp,
                                                               leap_seconds));
    }

    // Sort the list
    std::sort( m_posix_leap_second_table.begin(),
               m_posix_leap_second_table.end(),
               [](const std::tuple<double,double>&a,
                  const std::tuple<double,double>&b)->bool
                  {
                      return std::get<0>(a) < std::get<0>(b);
                  });

    // Set file as valid
    m_is_valid = true;
}


/***************************************************/
/*          Get the number of leap seconds         */
/***************************************************/
double Leap_Second_Table::Get_Leap_Seconds( const double& unix_timestamp )const
{
    // Check if we are smaller than the last timestamp
    if( std::get<0>(m_posix_leap_second_table[0]) > unix_timestamp ){
        return -1;
    }

    // Iterate over the list from start and find the first entry which is larger
    for( size_t i=1; i<m_posix_leap_second_table.size(); i++ )
    {
        if( std::get<0>(m_posix_leap_second_table[i]) > unix_timestamp )
        {
            // Get the last entry
            return std::get<1>(m_posix_leap_second_table[i]);
        }
    }
    return std::get<1>(m_posix_leap_second_table.back());

}


} // End of TM Namespace

