#include "string_utilities.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>

using namespace std;

/*
 * Trim a string of leading and trailing spaces. 
*/
std::string string_trim( const string& str ){
    
    string tstr = str;
    boost::algorithm::trim( tstr );
    return tstr;
}

/*
 * Split the string
 */
std::vector<std::string> string_split( const std::string& str, const std::string& pattern ){

    vector<string> output;
    boost::split( output, str, boost::is_any_of(pattern));
    return output;
}


/*
 * Convert string to lower case
 */
std::string string_toLower( const std::string& in ){
    
    std::string value = in;
    std::transform( in.begin(), in.end(), value.begin(), ::tolower);
    return value;
}

/*
 * Convert string to upper case
 */
std::string string_toUpper( const std::string& in ){

    std::string value = in;
    std::transform( in.begin(), in.end(), value.begin(), ::toupper);
    return value;
}


