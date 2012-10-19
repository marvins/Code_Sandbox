#ifndef __SRC_CAMERA_HPP__
#define __SRC_CAMERA_HPP__

///STL Libraries
#include <deque>
#include <map>
#include <set>
#include <string>
#include <vector>

///Personal Libraries
#include "FilePtr.hpp"
#include "Metrics.hpp"
#include "Options.hpp"
#include "IO.hpp"

using namespace std;


/**
 * @class TimeID
 * 
 * A basic structure which contains a directory path. 
 *
 * The structure contains the path which is decomposed into segments in a deque.  
 * This is the dir object.  The pathnames is the name of each camera base 
 * directory which contains that path. 
*/
class TimeID{

    public:
        /**
         * Default constructor. Initializes both deques to empty. 
        */
        TimeID();
        
        /**
         * Parameterized constructor. 
         *
         * Sets the directory name for the particular camera. 
         *
         * @param[in] dirname Name of the directory to decompose
         * @param[in] camname Name of the calling camera
        */
        TimeID( const string& dirname, const string& camname );

        /**
         * Decompose a path string into the proper beginning part and 
         * add it to the pathnames list.  
         *
         * This assumes that you have found a directory which is already in your
         * timespace tree and you want to append the new directory to the 
         * entry.  This will get rid of the cam###/COLLECT/HOUR/MIN stuff. 
        */
        void decompose_and_add_path( const string& str );
        
        static deque<string> get_image_path_parts( const string& str );

        bool operator < (  TimeID const& rh )const;
        bool operator > (  TimeID const& rh )const;
        bool operator == ( TimeID const& rh )const;
        bool operator != ( TimeID const& rh )const;
        
        deque<string> dirs;
        deque<string> pathnames;
};

ostream& operator << ( ostream& ostr, const TimeID& id );


/**
 * @class Camera
 *
 * The macro container for most imagery file structure information. 
*/
class Camera{

    public:
        
        /**
         * Default Constructor
        */
        Camera( );

        /**
         * Parameterized Constructor
         *
         * Inputs are the Camera ID and the starting directory name
        */
        Camera( const string& CAM_ID, const int& collectType, const string& directory_name );

        /**
         * Helper functions to determine if a camera path name is valid
        */
        static int isValid( string const& name, string const& cam_path_regex, string const& collect_name );
       
        /**
         *  Add a camera path to the root directory list
        */
        void add_directory( string const& dir_name );
       
        /**
         * Initialize the scene space for the camera, basically load a boatload of camera directories.
        */
        void build_scene_space( FilePtr const& newest_file, const int& image_depth );

        bool decompose_top_directories( );
        
        bool empty_time_space()const;

        static string create_CAM_ID( const string& cam_name );
        
        /**
         * Compare and prune the image list
         *
         * Compare the current image list against the test case and
         * remove all entries in the test case which are not in the
         * current image list. 
        */
        void union_image_list( deque<ImageBundle>& image_list )const;

        void build_scene_list( vector<SceneID>& scene_list, const int& current_idx, FilePtr& newest_file );

        deque<string> root_directories;  /*< Name of the base camera directory */
        
        TimeID last_time_entry;

        string CAM_ID;
        int collect_type;

        set<TimeID> time_space;
        deque<string> current_image_list;
};

/** 
 * Output stream operator 
*/
ostream& operator << ( ostream& ostr, Camera const& camera );


/**
 *
*/
class CameraSortFunc{
    
    public:
        
        bool operator ( ) ( const Camera& a, const Camera& b )const;
};

/**  
 * Convert the camera directory name into an index for the 
 * hash key. 
*/
int camera2int( const string& dirname );

/**
 * Gather all images from the top directory in each camera time space
 * and return a list of all valid image bundles. 
*/
deque<ImageBundle> decompose_top_camera_directories( deque<Camera>& cameras );

/**  
 * Find Camera Directories
 * 
 * Searches and loads all camera directories found in system
*/
deque<Camera> find_camera_directories( Options const& options );

/**
 * Make sure that each camera in the list has the same directory in 
 * the first position of its time space tree.
*/
void normalize_cameras( deque<Camera>& cameras );

/**
 * Compute the image bundles
*/
deque<ImageBundle> compute_image_bundles( deque<Camera>& cameras, Options const& options );

#endif

