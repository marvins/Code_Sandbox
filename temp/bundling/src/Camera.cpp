#include "Camera.hpp"

#include <sstream>

using namespace std;

void Camera::add_directory( string const& dir_name ){

    root_directories.push_back(dir_name);

}


/** 
 * Output stream operator 
*/
ostream& operator << ( ostream& ostr, Camera const& camera ){

    ///print header
    ostr << "Camera: " << endl;
    
    ///print root directories
    ostr << "   root directories: ";
    if( camera.root_directories.size() > 0 ){ 
        cout << camera.root_directories[0];
        for( size_t i=0; i<camera.root_directories.size(); i++)
            ostr << ", " << camera.root_directories[i];
    }
    ostr << endl;
    

    return ostr;
}

int camera2int( const string& dirname ){

    //pull out the last three
    string substr = dirname.substr(dirname.size()-3,3);
    
    int output;
    stringstream sin;
    sin << substr;
    sin >> output;
    return output;

}

bool isCharAlphaNumeric( char const& c ){

    if( c >= '0' && c <= '9' ){
        return true;
    }
    if( c >= 'A' && c <= 'F' )
        return true;

    return false;

}


bool Camera::isValid( string const& name ){
    
    //pull out the filename from the full path
    string fpath = file_basename(name);
    
    //we first need to ensure there is a cam in the first three letters
    if( fpath.substr(0,3) != "cam" )
        return false;
    
    //we need to make sure that the length is 6 characters
    if( fpath.size() != 6 )
        return false;
    
    //we need to make sure the last three values are alpha numeric
    if( isCharAlphaNumeric( fpath[3] ) == false ) return false;
    if( isCharAlphaNumeric( fpath[4] ) == false ) return false;
    if( isCharAlphaNumeric( fpath[5] ) == false ) return false;

    return true;
}

/**  
 * Find Camera Directories
 * 
 * Searches and loads all camera directories found in system
*/
deque<Camera> find_camera_directories( Options const& options ){
    
    //create a list of cameras
    deque<Camera> output( options.number_eo_frames );

    //create a list of directories to search
    deque<string> dir_tree(0);

    string cdir;

    /**
      start at the base directory and begin building a list of camera directories.  When 
      we discover one, add it to the respective camera.
    */
    dir_tree.push_back( options.input_base );
    while( true ){
        
        //pop the first item off of the stack
        cdir = dir_tree.front();
        dir_tree.pop_front();

        
        //check to see if it is a valid camera directory
        if( Camera::isValid(cdir)){
            
            //extract the camera number
            int camNum = camera2int( cdir );

            //add camera to the camera list
            output[camNum].root_directories.push_back(cdir);

        }
        else{

            //query the directory for all internal directories
            directory_append_internal( cdir, dir_tree, IO_DIRECTORIES_ONLY  ); 
        }

        //exit if we are out of directories
        if( dir_tree.size() <= 0 )
            break;
    }

    return output;

}

