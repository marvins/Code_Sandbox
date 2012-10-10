#ifndef __SRC_CAMERA_HPP__
#define __SRC_CAMERA_HPP__

///STL Libraries
#include <deque>
#include <map>
#include <set>
#include <string>
#include <vector>

///Personal Libraries
#include "Options.hpp"
#include "IO.hpp"

using namespace std;


class TimeID{

    public:
        TimeID();
        
        TimeID( const string& dirname, const string& camname );

        bool operator < (  TimeID const& rh )const;
        bool operator > (  TimeID const& rh )const;
        bool operator == ( TimeID const& rh )const;
        bool operator != ( TimeID const& rh )const;
        
        deque<string> dirs;
        deque<string> pathnames;
};

ostream& operator << ( ostream& ostr, const TimeID& id );


/** 
 * @class SceneID
 *
 * A basic structure for managing scene numbers
*/
class SceneID{
    
    public:
        SceneID();        
        SceneID( const string& name );

        bool operator < ( SceneID const& rh )const;
        bool operator > ( SceneID const& rh )const;
        bool operator == ( SceneID const& rh )const;
        bool operator != ( SceneID const& rh )const;

        string m_major;
        int    m_minor;
};

class Camera{

    public:

        static bool isValid( string const& name );
        
        void add_directory( string const& dir_name );
        
        void build_scene_space();
        
        deque<string> root_directories;  /*< Name of the base camera directory */
        
        string camera_name;

        set<TimeID> time_space;
        map<SceneID,string> scene_space;
};

/** 
 * Output stream operator 
*/
ostream& operator << ( ostream& ostr, Camera const& camera );


class ImageBundle{

    public:

        deque<string> data;

};

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

/**
 * Compute the image bundles
*/
map<int,ImageBundle> compute_image_bundles( deque<Camera>& cameras, Options const& options );

#endif

