/**
 * @file   StringUtilities.hpp
 * @author Marvin Smith
 * @date   10/2/2013
*/
#include <src/core/StringUtilities.hpp>

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

std::vector<std::string> string_split( const std::string& str, const std::string& pattern ){

    vector<string> output;
    boost::split( output, str, boost::is_any_of(pattern));
    return output;
}

