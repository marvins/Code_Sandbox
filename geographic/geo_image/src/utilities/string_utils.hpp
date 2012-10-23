#ifndef __SRC_UTILITIES_STRING_UTILS_HPP__
#define __SRC_UTILITIES_STRING_UTILS_HPP__

#include <sstream>
#include <string>

namespace GEO{
namespace STR{

std::string trim( std::string const& in );

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



std::string file_basename( std::string const& in );

} // end of STR
} // end of GEO

#endif 
