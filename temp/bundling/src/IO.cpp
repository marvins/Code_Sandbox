#include "IO.hpp"

#include <algorithm>
#include <boost/filesystem.hpp>

using namespace std;

namespace fs = boost::filesystem;

const int  FILES = 1;
const int  DIRECTORIES = 2;
const int  ALL = 3;

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

    sort( container.begin(), container.end());
}

string file_basename( const string& pathname ){

    return fs::path(pathname).filename().string();
}

std::string file_extension( const std::string& pathname ){
    return fs::extension( fs::path( pathname ));
}


/** 
 * Given some pathname, remove the last element.  For example, 
 * given /home/user/file.txt -> /home/user
 * given /home/user/files/   -> /home/user
*/
std::string file_pop_rear( string const& filename ){
    
  //strip the first part
  return fs::path( filename ).parent_path( ).string();

}
    
deque<string> file_decompose_path( string const& pathname ){

    //create path
    fs::path item( pathname );
    
    //create output
    deque<string> output;

    //iterate over item
    for (fs::path::iterator it(item.begin()), it_end(item.end()); it != it_end; ++it){
        output.push_back((*it).string());    
    }

    return output;
}


bool is_dir( const string& name ){
    return fs::is_directory( fs::path( name ) );
}

bool is_file( const string& name ){
    return fs::is_regular_file( fs::path( name ));
}

