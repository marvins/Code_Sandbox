#ifndef __SRC_CORE_FILE_UTILITIES_HPP__
#define __SRC_CORE_FILE_UTILITIES_HPP__


#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <deque>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

namespace fs=boost::filesystem;
namespace ba=boost::algorithm;

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
 * Delete a file
 */
void file_delete( string const& filename );

/**
 * Split the string
 */
vector<string> string_split( const string& line, const string& pattern );

/**
 * Return the list of items
 */
template <typename LISTTYPE>
LISTTYPE list_directory( const string& directory ){
    
    /// create the output
    LISTTYPE output;

    /// create the iterator
    for (fs::directory_iterator itr(directory); itr!=fs::directory_iterator(); ++itr){
        output.push_back( itr->string() );
    }
    
    return output;
}


std::string string_trim( std::string const& in );

std::string file_basename( const string& name );

std::string file_pathname( const string& name );

#endif
