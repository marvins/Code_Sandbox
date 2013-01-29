#ifndef __SRC_CORE_FILE_UTILITIES_HPP__
#define __SRC_CORE_FILE_UTILITIES_HPP__

#include <sstream>
#include <string>
#include <vector>

using namespace std;

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


/**
 * Check if the file exists
 */
bool file_exists( string const& filename );

/**
 * Split the string
 */
vector<string> string_split( const string& line, const string& pattern );

/**
 * Return the list of items
 */
vector<string> list_directory( const string& directory );

std::string string_trim( std::string const& in );


#endif
