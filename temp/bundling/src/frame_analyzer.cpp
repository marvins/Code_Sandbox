
///STL Libraries
#include <cstdio>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

///Personal Libraries
#include "Core.hpp"
#include "Options.hpp"
#include "Context.hpp"
#include "Metrics.hpp"

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
            
            //create the context object
            Context context( options.eval_use_context_file && options.eval_refresh_context);
            
            //create a container for our new metrics
            Metrics metrics;

            //skip find camera flag
            bool skip_finding_cameras = options.eval_use_context_file;

            /**
             * We must either find the camera directories or load them from the context.
            */
            if( skip_finding_cameras == true ){

                //load the context back into the flag just in case we mess up
                skip_finding_cameras = context.load_context( options.eval_context_filename );
                
            }
            
            // there may be a problem with the context file, so regardless of the config flag, we may need to rebuild the data
            if( skip_finding_cameras == false ){
                
                context.collect_type = options.collect_type;

                //lets find the camera directories
                context.cameras = find_camera_directories( options );
                
                // since the image depth is used in the context file, add it here from the config file
                context.image_depth = options.image_depth;
                context.metrics.camera_cnt = context.cameras.size();
            }

            /**
             * start searching through the image directories.
            */
            metrics = evaluate_frame_sets( context.cameras, options, context.newest_file );            
            
            //merge the results with the current metrics in the context
            context.metrics.merge( metrics );
            context.metrics.validate_scene_list( context.cameras.size() );

            /**
             * Now that we have completed our task, lets save the context back.
            */
            if( options.eval_use_context_file == true ){

               context.write_context( options.eval_context_filename ); 

            }

        }



    } catch (string e){
        cout << e << endl;
    }

    return 0;
}
