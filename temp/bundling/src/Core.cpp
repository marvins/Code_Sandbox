#include "Core.hpp"

using namespace std;


/**
 * Compute all required metrics for any new images created
 * since the last run.
*/
Metrics evaluate_frame_sets( deque<Camera>& cameras, Options& options ){
    
    for( int i=0; i<10; i++ )
        cout << cameras[i] << endl;
    cin.get();

    /**
     * Initialize the time space for every camera
    */
    for( size_t i=0; i<cameras.size(); i++ )
        cameras[i].build_scene_space();
    
    /**
     * Start searching the filesystem
    */
    bool run_loop = true;
    while( run_loop == true ){

    
        //for each directory
        cout << "TOP" << endl;
        for( size_t i=0; i<cameras.size(); i++ ){
            cout << cameras[i].time_space.size() << endl;
            cout << *(cameras[i].time_space.begin()) << endl;            
            cin.get();
        }
    
    }
     



/*
    // initialize each camera to have the same starting point
    deque<ImageBundle> bundle_output;
    deque<ImageBundle> bundles;

        
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
        
        cout << "C" << endl;
        //now decompose each directory and search for matching image pairs
        bundles.clear();
        bundles = decompose_top_camera_directories( cameras );
        
        cout << "E" << endl;
        //add the image bundles to the bundle list
        bundle_output.insert( bundle_output.end(), bundles.begin(), bundles.end() );
        
        cout << "Current Size: " << bundle_output.size() << endl;
        if( bundle_output.size() > options.max_bundle_limit )
        {
            cout << "hard limit reached" << endl;
            break;
        }

    }
    
*/


    
    // create a return metrics container
    Metrics metrics;




    return metrics;
}

