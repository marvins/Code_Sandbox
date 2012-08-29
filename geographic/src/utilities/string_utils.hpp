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

} // end of STR
} // end of GEO

#endif 
