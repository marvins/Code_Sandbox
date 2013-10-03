/**
 * @file   StringUtilities.hpp
 * @author Marvin Smith
 * @date   10/2/2013
*/
#ifndef __SRC_CORE_STRINGUTILITIES_HPP__
#define __SRC_CORE_STRINGUTILITIES_HPP__

#include <sstream>
#include <string>
#include <vector>


template <typename TP>
TP str2num( std::string const& value ){
    
    std::stringstream sin;
    TP result;
    sin << value;
    sin >> result;
    return result;
}

template <typename TP>
std::string num2str( TP const& value ){

    std::stringstream sin;
    sin << value;
    return sin.str();
}


/**
 * Trim a string of trailing and leading blanks
*/
std::string string_trim( const std::string& str );


/**
 * Split a string using a regex pattern
*/
std::vector<std::string> string_split( const std::string& str, const std::string& pattern );

#endif

