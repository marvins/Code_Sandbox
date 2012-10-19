#include "Core.hpp"
#include "FilePtr.hpp"
#include "ProgressBar.hpp"

#include <cstdlib>

using namespace std;


/**
 * Compute all required metrics for any new images created
 * since the last run.
*/
Metrics evaluate_frame_sets( deque<Camera>& cameras, Options& options, FilePtr& newest_file ){
    

    cout << endl << "Evaluating Frame Sets" << endl;
    
    /**
     * Initialize the time space for every camera
    */
    ProgressBar build_bar;
    cout << "Building Scene Space" << endl;
    for( size_t i=0; i<cameras.size(); i++ ){
        build_bar.update( (double)i/cameras.size() );
        cout << build_bar << flush;
        cameras[i].build_scene_space( newest_file, options.image_depth );
    }
    build_bar.update(1); 
    cout << build_bar << endl;
    cout << endl;

    // create a return metrics container
    Metrics metrics;
    
    // pass the metrics list with a table of indeces to CAM_IDS
    metrics.camera_names.clear();
    for( size_t i=0; i<cameras.size(); i++ ){
        metrics.camera_names.push_back( cameras[i].CAM_ID);
    }
    
    /**
     * Start searching the filesystem
    */
    bool empty_camera = true;
    vector<SceneID> scene_list;
    
    while( true ){
        
        
        //iterate through each camera and decompose each top directory into a list of images directory
        empty_camera = true;
        for( size_t i=0; i<cameras.size(); i++ ){
            if( cameras[i].decompose_top_directories() == true ){
                empty_camera = false;
            }
        }
        if( empty_camera == true )
            break;

        //clear the image scene list
        scene_list.clear();
        
        //analyze each camera for the scene list
        for( size_t i=0; i<cameras.size(); i++ )
            cameras[i].build_scene_list( scene_list, i, newest_file );

        //add all new scenes to the metrics tree
        metrics.append_scene_list( scene_list );
        
        cout << "\r                  \r Found " << metrics.scene_list.size() << " Frame Sets" << flush;
    }
    cout << endl;

    // iterate through all scenes and check if they are complete or not
    metrics.validate_scene_list( cameras.size() );
    

    return metrics;
}

