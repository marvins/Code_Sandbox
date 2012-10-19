#include "Camera.hpp"
#include "ProgressBar.hpp"
#include "TACID.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;



/**
 * Returns true if the character being evaluated
 * is either a number or an upper-case letter between
 * A and F inclusive. 
*/
bool isCharAlphaNumeric( char const& c ){
    
    // check if a number
    if( c >= '0' && c <= '9' ){
        return true;
    }
    //otherwise check if a letter
    if( c >= 'A' && c <= 'F' )
        return true;

    return false;

}



/** 
 * Default constructor for the TimeID class. 
*/
TimeID::TimeID(){
    dirs.clear();
    pathnames.clear();
}

/**
 * Parameterized constructor for the TimeID Class. 
*/
TimeID::TimeID( const string& dirname, const string& cam_id ){
    
    string camname = string("cam") + cam_id;

    // add directory to pathname list
    pathnames.push_back(dirname);
   
    //get the path elements in the form of a deque
    dirs = file_decompose_path( dirname );
        
    //look for the camname
    deque<string>::iterator pos = find( dirs.begin(), dirs.end(), camname );
        
    
    //if it is there, then delete everthing up to and including it for the dirname
    string pathStr = "";
    vector<string> pathstack;
    if( pos != dirs.end()){
        for( deque<string>::iterator it = dirs.begin(); it != pos; it++ ){
            
            //pull from the dir stack
            dirs.pop_front();
        }
        //remove the first item, as that is our starting directory
        dirs.pop_front();
    }
    
}


/**
 * Decompose the path string and add it to the pathnames list
*/
void TimeID::decompose_and_add_path( const string& pathStr ){

    //add to path string list
    pathnames.push_back( pathStr );
}


deque<string> TimeID::get_image_path_parts( const string& str ){

    //create output
    deque<string> output = file_decompose_path( str );
    
    //remove the early stuff
    while( output.size() > 0 ){ 
        
       //check if a camera directory
       if( output[0].size() == 6 && 
           output.front().substr(0,3) == "cam" &&
           isCharAlphaNumeric( output.front()[3] ) &&
           isCharAlphaNumeric( output.front()[4] ) &&
           isCharAlphaNumeric( output.front()[5] ) ){

            output.pop_front();
            break;
       }

       // otherwise just delete
       output.pop_front();

    }
    
    //remove the last item
    output.pop_back();

    return output;
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

/** 
 *  Output Stream operator for the TimeID class
*/
ostream& operator << ( ostream& ostr, const TimeID& id ){
    ostr << "DIRS: ";
    for( size_t i=0; i<id.dirs.size(); i++) 
        ostr << id.dirs[i] << " / ";
    ostr << endl;
    ostr << "PATHS: " << endl;
    for( size_t i=0; i<id.pathnames.size(); i++ )
        ostr << "   " << id.pathnames[i] << endl;

    return ostr;
}


/**
 * Default constructor
*/
Camera::Camera(){

}


/**
 * Parameterized constructor given a CAM_ID value and root directory entry.
*/
Camera::Camera( const string& cam_id, const int& collectType, const string& root_dir ){

    root_directories.push_back(root_dir);

    collect_type = collectType;

    CAM_ID = cam_id;

}


/**
 * Add a root directory to the camera object.
*/
void Camera::add_directory( string const& dir_name ){

    // add the filename to the root directory list
    root_directories.push_back(dir_name);

    // sort the list for sanity
    sort( root_directories.begin(), root_directories.end() );


}

/**
 * Takes the base pathnames provided to the camera object and builds a tree
 * structure consisting of the internal directories.  Since the camera has multiple
 * base directories, each subdirectory may exist in each base. 
*/
void Camera::build_scene_space( FilePtr const& newest_file, const int& image_depth ){
    
    // create our directory stack which we will perform a Depth-First-Search on.
    deque<string> image_stack;

    // load the directory stack with the contents of each base directory
    for( size_t i=0; i<root_directories.size(); i++ )
        directory_append_internal( root_directories[i], image_stack, IO_ALL);
    
    // some useful variables
    string topStr;
    
    // keep iterating until the image stack is empty
    while( image_stack.size() > 0 ){
        
        //pop off the first item
        topStr = image_stack.front();
        image_stack.pop_front();
        
        //if item is a directory, then add it to the directory tree and keep digging
        if( is_dir( topStr ) == true ){
            
            //create timeid
            TimeID id( topStr, CAM_ID );
            
            //check if the element already exists
            set<TimeID>::iterator it = time_space.find(id);

            if( it != time_space.end() ){
                
                TimeID actual = (*it);
                actual.decompose_and_add_path(topStr);
                time_space.erase(id);
                time_space.insert(actual);
            
            }
            else{
                

                //only add if the depth is less than the max
                //compute the depth
                int depth = id.dirs.size();

                //if the depth is larger than the newest file, file parts size, then ignore
                bool is_newer = true;
                
                
                ////////////////////////////////////////////////////////////////////
                // HISTORICAL PRUNING 
                
                if( newest_file.initialized == true && depth-1 > newest_file.file_parts.size() ){
                    is_newer = false;
                }

                deque<string> filepath = file_decompose_path( topStr );
                //TODO if depth == 0, then we are evaluating a date.  make sure the date is longer
                
                
                //if depth is anything else, normal comparison should work
                if( newest_file.initialized == true && depth-1 > 0 && depth-1 < newest_file.file_parts.size() &&
                    filepath.back() < newest_file.file_parts[depth-1] ){
                    is_newer = false;
                }
                
                // END OF HISTORICAL PRUNING
                ////////////////////////////////////////////////////////////////////

                if( is_newer && depth == image_depth ){
                    
                    //add to directory space
                    time_space.insert( TimeID( topStr, CAM_ID ) );
                }
                
                else if( is_newer && depth < image_depth ){

                    //keep adding
                    directory_append_internal( topStr, image_stack, IO_ALL );  
                }
            
            }
        }

    }
    
}

/** 
 * Pop the top entry off of the time space and retrieve all 
 * images. Place them onto the current image list.
 * Make sure to sort the list. 
*/
bool Camera::decompose_top_directories( ){
    
    // clear the current image list
    current_image_list.clear();
    
    //return false if we are out of time entris
    if( time_space.size() <= 0 )
        return false;

    // pop the top time space entry
    TimeID timeEntry = *time_space.begin(); 
    time_space.erase( time_space.begin());
    
    last_time_entry = timeEntry;


    // add the directory contents to the list
    for( size_t i=0; i<timeEntry.pathnames.size(); i++ ){
        directory_append_internal( timeEntry.pathnames[i], current_image_list, IO_FILES_ONLY );
    }
    
    // TODO Prune images with a later scene number than the last visited frame
    
    // sort the list by scene number
    sort_TACID_list( current_image_list, collect_type );

    return true;
}


/** 
 * Compares the current image list against the input
 * test case.  
*/
void Camera::union_image_list( deque<ImageBundle>& image_list )const{
    
    //compare each list side-by-side until image list has been iterated
    deque<ImageBundle>::iterator itA = image_list.begin();
    deque<string>::const_iterator itB = current_image_list.begin();
    int result;
    while( true ){
        
        //check if we have reached the end of the image list, if yes, then exit
        if( itA == image_list.end() )
            break;

        //if we reached the end of the current image list, 
        //   then remove the rest of the image list
        if( itB == current_image_list.end() ){
            while(itA != image_list.end() ){
                itA = image_list.erase(itA);
            }
        }

        //compare each tacid
        result = TACID_scene_func()( TACID( itA->data[0], collect_type), TACID(*itB, collect_type) );

        //if they are equal, add the image to the bundle and increment both pointers
        if( result == 0 ){
            
            itA->data.push_back(*itB);
            itA++;
            itB++;
            continue;
        }

        //if itA is smaller, delete it
        if( result < 0 ){
            itA = image_list.erase( itA );
            continue;
        }

        //if itB is smaller, than increment it
        else{
            itB++;
            continue;
        }

    }
}


/**
 * Return true if the camera is out of directories to search.
*/
bool Camera::empty_time_space()const{
    
    if( time_space.size() <= 0 )
        return true;
    
    return false;
}
    
/**
 * Given the scene list, update any frame which do or do not exist in either. 
*/
void Camera::build_scene_list( vector<SceneID>& scene_list, const int& current_idx, FilePtr& newest_file ){

    bool isValid = false;
    
    //iterate through the current image list. if the scene does not exist in the 
    //list, then add it
    for( size_t i=0; i<current_image_list.size(); i++ ){
        
        //grab the current scene number
        int tscene = TACID::scene_number( current_image_list[i], collect_type, isValid );
        
        //update the FilePtr if it is not initialized or not the largest
        if( newest_file.initialized == false || newest_file.scene_number < tscene ){
            newest_file.initialized = true;
            newest_file.file_parts = TimeID::get_image_path_parts( current_image_list[i] );
            newest_file.scene_number = tscene;
        }

        bool scene_exists = false;

        //check if it exists in the current scene list
        for( size_t j=0; j<scene_list.size(); j++ )
            if( scene_list[j].scene_number == tscene ){
                scene_list[j].camera_idx_list.push_back(current_idx);
                scene_exists = true;
                break;
            }
        
        //add the scene if it does not exist
        if( scene_exists == false )
            scene_list.push_back( SceneID( tscene, current_idx ));

    }

}


/** 
 * Output stream operator 
 */
ostream& operator << ( ostream& ostr, Camera const& camera ){

    ///print header
    ostr << "Camera: " << camera.CAM_ID << endl;

    ///print root directories
    ostr << "   root directories: " << endl;
    if( camera.root_directories.size() > 0 ){ 
        for( size_t i=0; i<camera.root_directories.size(); i++)
            ostr << "   " << camera.root_directories[i] << endl;
    }
    ostr << endl;


    return ostr;
}




/**
 *  Convert the cam### string into an actual 
 *  number which can be referenced in an array
*/
int camera2int( const string& dirname ){

    //pull out the last three
    string substr = dirname.substr(dirname.size()-3,3);

    int output;
    stringstream sin;
    sin << substr;
    sin >> output;
    return output;

}

/**
 *  Compare two strings and see if they match
*/
bool string_match( const string& filename, const string& regex_name ){
    
    // if the regex is a single astrick, then pass
    if( regex_name == "*" )
        return true;
    
    //make sure they are the same size
    if( filename.size() != regex_name.size() )
        return false;
    
    // iterate over the regex_name
    for( size_t i=0; i<filename.size(); i++ ){

        // if the regex has a #, then pass automatically
        if( regex_name[i] == '#' )
            continue;

        // otherwise, compare
        if( filename[i] != regex_name[i] )
            return false;
    }

    return true;

}


/**
 * checks to make sure that the string matches the requirements
 * to be a camera directory.  The main requirement is that it follows
 * the general template  cam### where ### is a 3 digit hex number.
*/
int Camera::isValid( string const& name, string const& cam_path_regex, string const& collect_name ){

    int result = 1;
    
    //split the path into components
    deque<string> path_parts = file_decompose_path( name );

    //split the name into components
    deque<string> regx_parts = file_decompose_path( cam_path_regex );

    //compare each pair of elements
    for( size_t i=0; i<path_parts.size(); i++ ){
        
        // make sure that we still have elements in the regx parts
        //  if not, then the path is longer than the regx.  That means we need to quit
        if( i >= regx_parts.size() ){
            result = -1;
            break;
        }

        // check if the regex part is _COLLECT_ as this represent the collect name and not the regex
        if( regx_parts[i] == "_COLLECT_" )
            regx_parts[i] = collect_name;

        // check that the elements compare successfully
        //   if not, then we should not proceed any further
        if( string_match( path_parts[i], regx_parts[i] ) == false ){
            result = -1;
            break;
        }

    }

    // if the path parts is shorter than the regx parts, then the name string is a sub-path of the regx path
    if( result == 1 && path_parts.size() < regx_parts.size() ){
        result = 0;
    } 

    return result;
}


/**
 *  Create a TAG ID for the input camera file
*/
string Camera::create_CAM_ID( const string& directory_name ){
    return directory_name.substr(directory_name.size()-3);
}
    
    
bool CameraSortFunc::operator ( ) ( const Camera& a, const Camera& b )const{
    return a.CAM_ID < b.CAM_ID;
}


/***********************************************************************/
/*                                                                     */
/***********************************************************************/
/**  
 * Find Camera Directories
 * 
 * Searches and loads all camera directories found in system
 *
 * Algorithm:
 * - Determine if we are looking for raws or nitfs
 *   1. RAW Algorithm
 *      a.  Starting with the base directory, search for every camera directory
 */
deque<Camera> find_camera_directories( Options const& options ){

    //create a list of cameras
    deque<Camera> output(0);

    //create a list of directories to search
    deque<string> dir_tree(0);

    //create a camera list structure
    deque<string> camera_list;

    string cdir;
    
    cout << "Finding Cameras" << endl;
    /**
      start at the base directory and begin building a list of camera directories.  When 
      we discover one, add it to the respective camera.
      */
    dir_tree.push_back( options.input_base );
    while( true ){

        //pop the first item off of the stack
        cdir = dir_tree.front();
        dir_tree.pop_front();


        /* Check to see if the directory matches the pattern */
        int dir_state = Camera::isValid(cdir, options.input_base + string("/") + options.collect_camera_path, options.collect_name);
        
        if( dir_state == 1 ){
        
            //place directory name onto camera list
            camera_list.push_back( cdir );

        }
        else if( dir_state == 0 ){

            //query the directory for all internal directories
            directory_append_internal( cdir, dir_tree, IO_DIRECTORIES_ONLY  ); 
        }

        
        //exit if we are out of directories
        if( dir_tree.size() <= 0 )
            break;
    }

    
    /** 
     * sort list by camera number
    */
    sort( camera_list.begin(), camera_list.end(), camlist_sort() );

    /**
     * convert to camera objects
    */
    bool camera_found;
    while( camera_list.size() > 0 ){
        
        //retrieve the next camera name
        string tname = camera_list.back();
        camera_list.pop_back();

        //create a tag ID to test against
        string CAM_ID = Camera::create_CAM_ID( tname );
        
        //check if there is a camera with the same CAM_ID
        camera_found = false;
        for( int j=(int)output.size()-1; j>=0; j-- ){
            
            // if the CAM_IDs match, then append the directory to the current camera
            if( output[j].CAM_ID == CAM_ID ){
                output[j].add_directory( tname );
                camera_found = true;
                break;
            }

        }

        // if no camera was found with the CAM_ID, then create a camera
        if( camera_found == false ){
            output.push_front( Camera(CAM_ID, options.collect_type, tname));
        }
        cout << '\r' << "                            " << '\r' << "Found: " << output.size() << flush;
        
    }
    cout << endl;

    return output;

}


/**
 * Make sure each camera in the list has the same directory 
 * at the top of its time space. 
*/
void normalize_cameras( deque<Camera>& cameras ){

    bool inSync;
    
    //search until we have a level set of cameras
    while( true ){

        inSync = true;
        for( size_t i=1; i<cameras.size(); i++){

            //if the camera list is empty, then exit
            if( cameras[0].time_space.size() <= 0 || cameras[i].time_space.size() <= 0 ){
                return;
            }

            //keep moving if the elements are equal
            if( *cameras[0].time_space.begin() == *cameras[i].time_space.begin() )
                continue;

            //otherwise, we are out of sync and must correct
            inSync = false;

            //find the smaller element and remove it.
            if(  (*cameras[0].time_space.begin()) < (*cameras[i].time_space.begin()) )
                cameras[0].time_space.erase(cameras[0].time_space.begin());
            else
                cameras[i].time_space.erase(cameras[i].time_space.begin());

            break;
        }

        if( inSync == true )
            break;

    }

}


/**
 * Decompose and pull all image sets for the selected directory
*/
deque<ImageBundle> decompose_top_camera_directories( deque<Camera>& cameras ){

    //create output
    deque<ImageBundle> output;
    
    //iterate through each camera and decompose each directory
    for( size_t i=0; i<cameras.size(); i++ )
        cameras[i].decompose_top_directories();
    
    //using the first image list as the base, union each image list set based
    // on scene number. 
    deque<string> imgList = cameras[0].current_image_list;
    
    
    for( deque<string>::iterator it=imgList.begin(); it != imgList.end(); it++ )
        output.push_back( ImageBundle(*it, cameras[0].collect_type));

    
    for( size_t i=1; i<cameras.size(); i++ )
        cameras[i].union_image_list( output );
    

    //return output
    return output;
}


/** 
 * Grab all matching and complete image bundles
*/
deque<ImageBundle> compute_image_bundles( deque<Camera>& cameras, Options const& options ){

    
    deque<ImageBundle> bundle_output;
    deque<ImageBundle> bundles;

    // For each camera, initialize the time space
    //for( size_t i=0; i<cameras.size(); i++ ){
    //    cameras[i].build_scene_space();
    //}

    int cnt = 0;
    /** Begin comparing directories */
    bool run_loop = true;
    while( run_loop == true ){
        
        //stop processing if any camera node is empty
        for( size_t i=0; i<cameras.size(); i++ ){
            if( cameras[i].empty_time_space() == true ){
                run_loop = false; 
                break;
            }
        }
        if( run_loop == false )
            break;
        
        //first check to make sure all cameras have the same top element
        normalize_cameras( cameras );
        
        //stop processing if any camera node is empty
        for( size_t i=0; i<cameras.size(); i++ ){
            if( cameras[i].empty_time_space() == true ){
                run_loop = false;
                break;
            }
        }
        if( run_loop == false )
            break;
        
        //now decompose each directory and search for matching image pairs
        bundles.clear();
        bundles = decompose_top_camera_directories( cameras );
        
        //add the image bundles to the bundle list
        bundle_output.insert( bundle_output.end(), bundles.begin(), bundles.end() );
        
        if( bundle_output.size() > options.max_bundle_limit )
        {
            break;
        }

    }
    
    
    return bundle_output;
}

