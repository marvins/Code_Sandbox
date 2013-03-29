#include "string_utilities.hpp"

#include <boost/algorithm/string.hpp>

using namespace std;

/**
 * Trim a string of leading and trailing spaces. 
*/
std::string string_trim( const string& str ){
    
    string tstr = str;
    boost::algorithm::trim( tstr );
    return tstr;
}
