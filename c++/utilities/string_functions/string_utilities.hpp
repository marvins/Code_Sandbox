#ifndef _STRING_UTILITIES_HPP__
#define _STRING_UTILITIES_HPP__

#include <sstream>
#include <string>

/**
 * Convert a string into a number
 *
 * @param[in] value String value to convert
 * 
 * @return Numerical equivalent
 */
template <typename TP>
TP str2num( std::string const& value ){
    
    std::stringstream sin;
    TP result;
    sin << value;
    sin >> result;
    return result;
}

/**
 * Convert a number into a string
 *
 * @param[in] value Numerical value to convert
 *
 * @return String representation
 */
template <typename TP>
std::string num2str( TP const& value ){

    std::stringstream sin;
    sin << value;
    return sin.str();
}


/**
 * Trim whitespace from the ends of a string
 *
 * @param[in] str  String to trim
 *
 * @return Trimmed string
*/
std::string string_trim( const std::string& str );

/**
 * Split a string using a pattern
 *
 * @param[in] str  String to split
 * @param[in] pattern Regex pattern to split by
 *
 * @return list of strings which were split from the input.
*/
std::vector<std::string> string_split( const std::string& str, const std::string& pattern );


/**
 * Convert string to lower-case
 *
 * @param[in] in input string
 *
 * @return lower-case string
 */
std::string string_toLower( std::string const& in );

/**
 * Convert string to upper-case
 *
 * @param[in] in input string
 *
 * @return upper-case string
 */
std::string string_toUpper( std::string const& in );




#endif
