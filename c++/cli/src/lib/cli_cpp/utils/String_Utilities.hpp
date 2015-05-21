/**
 * @file    String_Utilities.hpp
 * @author  Marvin Smith
 * @date    5/20/2015
 */
#ifndef __CLI_UTILS_STRING_UTILITIES_HPP__
#define __CLI_UTILS_STRING_UTILITIES_HPP__

// C++ Standard Libraries
#include <cinttypes>
#include <sstream>
#include <string>


namespace UTILS{

/**
 * @brief Convert a Number to a String.
 *
 * @param[in] value Value to convert.
 *
 * @return String format.
 */
template <typename TP>
std::string num2str( TP const& value )
{
    std::stringstream sin;
    sin << value;
    return sin.str();
}


/**
 * @brief Convert a String into a Number.
 *
 * @param[in] value to convert.
 *
 * @return number format.
 */
template <typename TP>
TP str2num( const std::string& value )
{
    std::stringstream sin;
    sin << value;
    TP output;
    sin >> output;
    return output;
}


/**
 * @enum StringAlignment
 */
enum class StringAlignment : uint8_t{
    CENTER = 0,
    LEFT   = 1,
    RIGHT  = 2,
}; // End of StringAlignment Enum Class

/**
 * @enum StringDirection
 */
enum class StringDirection : uint8_t{
    LEFT  = 0,
    RIGHT = 1,
    BOTH  = 2,
}; // End of StringDirection Enum Class


/**
 * @brief String Strip
 */
std::string String_Trim( const std::string&     data,
                         const std::string&     strip_patterns = "\n\0 ",
                         const StringDirection& strip_direction = StringDirection::BOTH );

                
/**
 * @brief String Fill
*/
std::string String_Fill( const std::string&      data,
                         const char&             fill_char,
                         const int&              fill_cnt,
                         const StringDirection&  fill_direction = StringDirection::RIGHT );


/**
 * @brief Format a string
 *
 * @param[in] data Data to align
 * @param[in] alignment Alignment format.
 */
std::string Format_String( const std::string&     data,
                           const int&             output_size,
                           const StringAlignment& alignment = StringAlignment::CENTER,
                           const char&            fill_char = ' ' );


} // End of UTILS Namespace

#endif
