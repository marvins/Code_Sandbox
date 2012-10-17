
///STL Libraries
#include <cstdio>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

///Personal Libraries
#include "Camera.hpp"
#include "Options.hpp"


///useful namespaces
using namespace std;


/** 
 * Main Driver
 *
 * Implements the primary status interface
*/
int main( int argc, char * argv[] ){
    
    try{

        ///load the configuration here
        Options options( argc, argv );
    
        ///print the configuration
        options.print();
        
        /**
         * Split into two separate programs depending on program mode
         *
         * if PROGRAM_BUNDLE, perform full camera search, and compute_image_bundles
         * if PROGRAM_EVAL, load context if it exists and compute metrics
        */
        if( options.program_mode == PROGRAM_BUNDLE ){

            ///find the camera directories
            deque<Camera> cameras = find_camera_directories( options );

            ///build a list of image bundles
            deque<ImageBundle> bundles = compute_image_bundles( cameras, options );
        
            ///Compress and ship files
            compress_bundles( bundles, options );
        
        }
        else if( options.program_mode == PROGRAM_EVAL ){
            
            //if there is no context, then look for a new set of cameras
            deque<Camera> cameras = find_camera_directories( options );

            //if there is a context or history, then load the camera list
            
            cout << "Cameras" << endl;
            for( size_t i=0; i<cameras.size(); i++ ){
                cout << cameras[i] << endl;
                cin.get();
            }

        }



    } catch (string e){
        cout << e << endl;
    }

    return 0;
}
