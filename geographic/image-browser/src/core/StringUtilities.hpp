/**
 * @file   StringUtilities.hpp
 * @author Marvin Smith
 * @date   10/2/2013
*/
#ifndef __SRC_CORE_STRINGUTILITIES_HPP__
#define __SRC_CORE_STRINGUTILITIES_HPP__

#include <string>
#include <vector>

/**
 * Trim a string of trailing and leading blanks
*/
std::string string_trim( const std::string& str );


/**
 * Split a string using a regex pattern
*/
std::vector<std::string> string_split( const std::string& str, const std::string& pattern );

#endif

