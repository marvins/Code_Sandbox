#ifndef __SRC_CAMERA_HPP__
#define __SRC_CAMERA_HPP__

///STL Libraries
#include <string>
#include <deque>
#include <vector>

///Personal Libraries
#include "Options.hpp"
#include "IO.hpp"

using namespace std;

class Camera{

    public:

        static bool isValid( string const& name );
        
        void add_directory( string const& dir_name );
        vector<string> root_directories;  /*< Name of the base camera directory */

};

/** 
 * Output stream operator 
*/
ostream& operator << ( ostream& ostr, Camera const& camera );


/**  
 * Convert the camera directory name into an index for the 
 * hash key. 
*/
int camera2int( const string& dirname );


/**  
 * Find Camera Directories
 * 
 * Searches and loads all camera directories found in system
*/
deque<Camera> find_camera_directories( Options const& options );

#endif

