#ifndef __STRING_UTILS_HPP__
#define __STRING_UTILS_HPP__

#include <sstream>
#include <string>

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


#endif

