/**
 *  Common String Utilities
 */
#ifndef __UTILS_STRING_UTILITIES_HPP__
#define __UTILS_STRING_UTILITIES_HPP__

#include <sstream>
#include <string>

/**
 *  Convert a string to a number
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
 *  Convert a number to a string
 */
template <typename TP>
std::string num2str( TP const& value ){

    std::stringstream sin;
    sin << value;
    return sin.str();
}

#endif
