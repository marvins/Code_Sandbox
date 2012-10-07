#include "IO.hpp"

#include <boost/filesystem.hpp>

using namespace std;

namespace fs = boost::filesystem;

const int  FILES = 0;
const int  DIRECTORIES = 1;


void directory_append_internal( const string& filename, deque<string>& container, const int& flag ){

    //create a path
    fs::path dirname( filename );

    //make sure it exists
    if( fs::exists( dirname ) == false )
        throw string( "ERROR: directory does not exist");

    //iterate through it and pull out all files matching the flag description
    fs::directory_iterator iter(dirname);
    fs::directory_iterator end_iter;
    
    //flag, 0 is file, 1 is directory
    for( ; iter != end_iter; iter++){
        
        string entry = (*iter).path().string();
        //get the file type
        if( (flag & DIRECTORIES) == DIRECTORIES && fs::is_directory( *iter ) == true ){
            container.push_front( entry );    
        }
        else if( (flag & FILES) == FILES && fs::is_regular_file( *iter ) == true ){
            container.push_front( entry );    
        }
    }
}

string file_basename( const string& pathname ){

    return fs::path(pathname).filename().string();
}

