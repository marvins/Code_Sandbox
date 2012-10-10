#include "Camera.hpp"
#include "TACID.hpp"

#include <algorithm>
#include <sstream>

using namespace std;

TimeID::TimeID(){
    dirs.clear();
    pathnames.clear();
}

TimeID::TimeID( const string& dirname, const string& camname ){

    //get the path elements
    dirs = file_decompose_path( dirname );
        
    //look for the camname
    deque<string>::iterator pos = find( dirs.begin(), dirs.end(), camname );
    

    //if it is there, then delete everthing up to and including it
    if( pos != dirs.end()){
        for( deque<string>::iterator it = dirs.begin(); it != pos; it++ ){
            dirs.pop_front();
        }
        dirs.pop_front();
    }

}


bool TimeID::operator <(  TimeID const& rh )const{
    
    //iterate through the host directories
    for( size_t i=0; i<dirs.size(); i++){

        //make sure the guest still has directories
        //  if not, then the host is smaller
        if( rh.dirs.size() <= i ){
            return true;
        }

        //compare directories
        if( dirs[i] < rh.dirs[i] )
            return true;
        else if( dirs[i] > rh.dirs[i] )
            return false;
        else 
            continue;

    }

    //if they are equal and the guest has more directories, then the host is smaller
    if( dirs.size() < rh.dirs.size() )
        return true;
    
    return false;
}

bool TimeID::operator >(  TimeID const& rh )const{

    //iterate through the host directories
    for( size_t i=0; i<dirs.size(); i++){

        //make sure the guest still has directories
        //  if not, the host is larger
        if( rh.dirs.size() <= i ){
            return true;
        }

        //compare directories
        if( dirs[i] > rh.dirs[i] )
            return true;
        else if( dirs[i] < rh.dirs[i] )
            return false;
        else 
            continue;

    }

    //if they are equal and the guest has more directories, then the host is smaller
    if( dirs.size() < rh.dirs.size() )
        return false;

    //otherwise they are equal
    return false;
}

bool TimeID::operator ==( TimeID const& rh )const{

    //make sure they have the same size
    if( dirs.size() != rh.dirs.size() )
        return false;
    
    //check to make sure the contents are equal
    for( size_t i=0; i<dirs.size(); i++ ){
        
        if( dirs[i] != rh.dirs[i] )
            return false;
    }

    return true;

}
bool TimeID::operator !=( TimeID const& rh )const{
    
    //make sure they have the same size
    if( dirs.size() != rh.dirs.size() )
        return true;

    //check to make sure the contents are equal
    for( size_t i=0; i<dirs.size(); i++ ){
        
        if( dirs[i] != rh.dirs[i] )
            return true;
    }

    return false;

}

ostream& operator << ( ostream& ostr, const TimeID& id ){
    ostr << "PATH: ";
    for( size_t i=0; i<id.dirs.size(); i++) 
        cout << id.dirs[i] << " / ";
    
    return ostr;
}

SceneID::SceneID( ): m_major("0"), m_minor(0){}

SceneID::SceneID( string const& filename ){

    m_major = "0";
    
    //pull out scene number
    bool isValid;
    m_minor = TACID::scene_number( filename, isValid );

}


bool SceneID::operator < ( SceneID const& rh )const{
    if( m_major == rh.m_major ){
        if( m_minor < rh.m_minor ) return true;
    }
    else if( m_major < rh.m_major )
        return true;
        
    return false;
}

bool SceneID::operator > ( SceneID const& rh )const{
    if( m_major == rh.m_major ){
        if( m_minor > rh.m_minor ) 
            return true;
    }
    else if( m_major > rh.m_major )
        return true;
        
    return false;
}

bool SceneID::operator == ( SceneID const& rh )const{
    if( m_major == rh.m_major )
        if( m_minor == rh.m_minor ) return true;
    return false;
}

bool SceneID::operator != ( SceneID const& rh )const{

    if( m_major == rh.m_major )
        if( m_minor == rh.m_minor ) return false;
    return true;

}

void Camera::add_directory( string const& dir_name ){

    root_directories.push_back(dir_name);

}

void Camera::build_scene_space(){
    
    //
    deque<string> image_stack;
    for( size_t i=0; i<root_directories.size(); i++ )
        directory_append_internal( root_directories[i], image_stack, IO_ALL);
    
    string topStr;
    SceneID id;
    
    while( image_stack.size() > 0 ){
        
        //pop off the first item
        topStr = image_stack.front();
        image_stack.pop_front();

        //if item is a directory, then add it to the directory tree and keep digging
        if( is_dir( topStr ) == true ){

            //create timeid
            TimeID id( topStr, camera_name );

            //check if the element exists
            set<TimeID>::iterator it = time_space.find(id);

            if( it != time_space.end() ){
                TimeID actual = (*it);
                actual.pathnames.push_back(topStr);
                time_space.erase(id);
                time_space.insert(actual);
            }
            else{

                //only add if the depth is less than the max
                //compute the depth
                int depth = TimeID( topStr, camera_name).dirs.size();
                if( depth == 3 ){
                    
                    //add to directory space
                    time_space.insert( TimeID( topStr, camera_name ) );
                }
                
                if( depth < 3 ){
                    //keep adding
                    directory_append_internal( topStr, image_stack, IO_ALL );  
                }
            }
        }

    }
    cout << "Camera Time Space: " << time_space.size() << " elements" << endl;

/*
    set<TimeID>::iterator it = time_space.begin();
    for( size_t i=0; i<10; i++){
        cout << (*it) << endl;
        it++;
    }
*/

}

/** 
 * Output stream operator 
 */
ostream& operator << ( ostream& ostr, Camera const& camera ){

    ///print header
    ostr << "Camera: " << endl;

    ///print root directories
    ostr << "   root directories: " << endl;
    if( camera.root_directories.size() > 0 ){ 
        for( size_t i=0; i<camera.root_directories.size(); i++)
            ostr << "   " << camera.root_directories[i] << endl;
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
            output[camNum].camera_name = file_basename( cdir );

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

map<int,ImageBundle> compute_image_bundles( deque<Camera>& cameras, Options const& options ){

    map<int,ImageBundle> bundles;

    // For each camera, initialize the search space
    for( size_t i=0; i<cameras.size(); i++ ){
        cout << i << " : ";
        cameras[i].build_scene_space();
    }

    // 

    return bundles;
}

