#ifndef _STRING_UTILITIES_HPP__
#define _STRING_UTILITIES_HPP__

#include <sstream>
#include <string>

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
 * Trim whitespace from the ends of a string
*/
std::string string_trim( const std::string& str );

#endif
