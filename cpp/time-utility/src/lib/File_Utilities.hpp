/**
 * @file    File_Utilities.hpp
 * @author  Marvin Smith
 * @date    10/19/2015
*/
#ifndef __TIME_UTILITIES_LIB_FILE_UTILITIES_HPP__
#define __TIME_UTILITIES_LIB_FILE_UTILITIES_HPP__

// C++ Libraries
#include <string>
#include <vector>

namespace TM{

/**
 * @brief Parse File Contents.
 * 
 * @param[in] pathname 
 *
 * @return Contents of file.
*/
std::string Load_Binary_File( const std::string& pathname );

/**
 * @brief Parse CSV File
 *
 * @param[in] pathname  File to load.
 *
 * @return List of CSV Lines.
*/
std::vector<std::vector<std::string>> Parse_CSV_File( const std::string&        pathname,
                                                      std::vector<std::string>& header,
                                                      const bool&               load_headers = false );

} // End of TM Namespace

#endif
