#include "FileUtilities.hpp"

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <iostream>
#include <string>

namespace fs=boost::filesystem;
namespace ba=boost::algorithm;

using namespace std;

/**
 * Check if the file exists
 */
bool file_exists( string const& filename ){
    return fs::exists( fs::path( filename ) );
}


vector<string> string_split( string const& line, const string& pattern ){

    string pre_split = line;
    vector<string> substrs;

    ba::split( substrs, pre_split, ba::is_any_of(pattern));
    return substrs;

}

vector<string> list_directory( const string& directory ){
    
    /// create the output
    vector<string> output;

    /// create the iterator
    for (fs::directory_iterator itr(directory); itr!=fs::directory_iterator(); ++itr){
        output.push_back( itr->string() );
    }
    
    return output;
}

std::string string_trim( std::string const& in ){
    
    string out = in;
    ba::trim( out );
    return out;
}

    
