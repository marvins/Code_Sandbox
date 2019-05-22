/**
 * @file    String_Utilities.hpp
 * @author  Marvin Smith
 * @date    1/21/2019
 */
#ifndef OPENCV_STAB_DEMO_STRING_UTILITIES_HPP
#define OPENCV_STAB_DEMO_STRING_UTILITIES_HPP

// C++ Libraries
#include <algorithm>
#include <string>
#include <vector>

// Boost Libraries
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


/**
 * @enum StringDirection
 */
enum class StringDirection : uint8_t{
    LEFT  = 1,
    RIGHT = 2,
    BOTH  = 3,
}; // End of StringDirection Enum Class


/**
 * @brief Split a string.
 *
 * @param[in] data Data to split.
 * @param[in] pattern Pattern to split the string on.
 *
 * @return List of components of split string.
 */
std::vector<std::string>  String_Split( const std::string& data,
                                        const std::string& pattern );


/**
 * @brief Split a List of Strings into a List of Something Else.
 *
 * @param[in] data
 */
template <typename DATA_TYPE,
        typename LIST_TYPE>
std::vector<DATA_TYPE> Convert_String_List( const LIST_TYPE& data )
{

    // Create output list
    std::vector<DATA_TYPE> output;
    std::transform( data.begin(),
                    data.end(),
                    std::back_inserter(output),
                    []( const std::string& s){ return boost::lexical_cast<int>(s); });
    return output;
}


/**
 * @brief Trim the string on both ends.
 *
 * @param[in] data String to trim.
 *
 * @return Trimmed string.
 */
std::string String_Trim( const std::string&  data );


/**
 * @brief Trim the string on both ends given the strip pattern.
 *
 * @param[in] data
 * @param[in] strip_pattern
 *
 * @return Trimmed string.
 */
std::string String_Trim( const std::string&  data,
                         const std::string&  strip_pattern );


/**
 * @brief Trim a string.
 *
 * @param[in] data Data to trim.
 * @param[in] strip_patterns Pattern to strip.
 * @param[in] strip_direction Direction to strip.
 *
 * @return Resulting string.
 */
std::string String_Trim( const std::string&     data,
                         const std::string&     strip_patterns,
                         const StringDirection& strip_direction);


/**
 * @brief Format a String.
 */
std::string Format_String( const std::string& input_data,
                           const uint64_t&    max_width );

#endif