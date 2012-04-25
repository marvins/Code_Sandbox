#include "string_utilities.h"

#include <sstream>

int str2int( std::string const& value ){
    std::stringstream sin;
    int result;
    sin << value;
    sin >> result;
    return result;
}

std::string int2str( int const& value ){

    std::stringstream sin;
    sin << value;
    return sin.str();
}

