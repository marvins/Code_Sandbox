/**
 * @file   FilesystemUtilities.cpp
 * @author Marvin Smith
 * @date   10/1/2013
 */
#include "FilesystemUtilities.hpp"


#ifndef Q_MOC_RUN
#include <boost/filesystem.hpp>
#endif

namespace fs = boost::filesystem;

using namespace std;

/**
 * Check whether or not a file exists.
*/
bool file_exists( string const& filename ){
    return fs::exists( fs::path( filename ) );
}

/**
 * Return the extension of the file
*/
string file_extension( string const& pathname ){
    return fs::extension( fs::path( pathname ) );

}

/**
 * Return the filename component of the path
*/
std::string file_basename( const std::string& filename ){
    return fs::path(filename).filename();
}

/**
 * Return the current working directory
*/
std::string current_working_directory(){
    return fs::current_path().string();
}

/**
 * Return the canonical path 
 */
std::string file_canonical( const string& filename ){
    return fs::canonical( fs::path(filename)).string();
}
