#ifndef __SRC_UTILITIES_STRING_UTILS_HPP__
#define __SRC_UTILITIES_STRING_UTILS_HPP__

#include <sstream>
#include <string>
#include <vector>

namespace GEO{
namespace STR{

std::string string_trim( std::string const& in );

std::vector<std::string> string_split( std::string const& original, std::string const& pattern );

std::string string_toLower( std::string const& in );

std::string string_toUpper( std::string const& in );

template <typename TP>
TP str2num( std::string const& in ){
    
    TP out;
    std::stringstream(in) >> out;
    return out;

}

template <typename TP>
std::string num2str( TP const& value ){

    std::stringstream sin;
    sin << value;
    return sin.str();
}




} // end of STR
} // end of GEO

#endif 
