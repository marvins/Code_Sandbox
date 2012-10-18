#include "Core.hpp"

using namespace std;


/**
 * Compute all required metrics for any new images created
 * since the last run.
*/
Metrics evaluate_frame_sets( deque<Camera>& cameras, Options& options ){

    /**
     * Initialize the time space for every camera
    */
    for( size_t i=0; i<cameras.size(); i++ )
        cameras[i].build_scene_space();
    
    
    // create a return metrics container
    Metrics metrics;
    
    /**
     * Start searching the filesystem
    */
    bool empty_camera = true;
    vector<SceneID> scene_list;
    
    while( true ){
        
        
        //iterate through each camera and decompose each top directory into a list of images directory
        empty_camera = true;
        for( size_t i=0; i<cameras.size(); i++ )
            if( cameras[i].decompose_top_directories() == true ){
                empty_camera = false;
                break;
            }
        if( empty_camera == true )
            break;
        
        // TODO  validate top directories.  If the top directories don't match, everything will get messy


        //clear the image scene list
        scene_list.clear();
        
        //analyze each camera for the scene list
        for( size_t i=0; i<cameras.size(); i++ )
            cameras[i].build_scene_list( scene_list, i );

        //add all new scenes to the metrics tree
        metrics.append_scene_list( scene_list );
        
    }

    // iterate through all scenes and check if they are complete or not
    metrics.validate_scene_list( cameras.size() );
    
    // pass the metrics list with a table of indeces to CAM_IDS
    metrics.camera_names.clear();
    for( size_t i=0; i<cameras.size(); i++ )
        metrics.camera_names.push_back( cameras[i].CAM_ID);
    

    return metrics;
}

