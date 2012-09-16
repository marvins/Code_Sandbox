/**
 * Orthoprojection Test Program
 *
 * @Author Marvin Smith, Sierra Nevada Corporation
 * @Date  
*/

//STL Libraries
#include <iostream>

//OpenCV Libraries
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//Personal Libraries
#include "core/Configuration.hpp"
#include "ortho/Orthorectify.hpp"
#include "ortho/Perspective.hpp"

//Relevant Namespaces
using namespace cv;
using namespace std;



/**
 * Main Driver
*/
int main( int argc, char* argv[] ){
    
    //outer try loop. Designed to not be required. Anything here has made it past logging.
    try{

        // Load and open the configuration file
        Options options(argc, argv);

        // main try loop.  Most if not all messages should be stopped here as logging will grab it
        try{

            //print configuration to the screen
            options.print();
            
            throw string("END");

            //if we need to build an image, run here
            if( options.get_run_type() == "FULL" || options.get_run_type() == "BUILD" ){

                //generate our perspective image
                options.image = generate_perspective_test_image( options );
                
                //write and display image
                namedWindow("Modified Image");
                imshow("Modified Image", options.image);
                waitKey(0);
                imwrite("data/modified_image.jpg", options.image);
                destroyWindow("Modified Image");

            }
            //if we need to rectify an image, run here
            if( options.get_run_type() == "FULL" || options.get_run_type() == "RECTIFY" ){

                //rectify the image
                Mat corrected_image = orthorectify( options.image, options );  

                //write and display image
                namedWindow("Fully Rectified Image");
                imshow("Fully Rectified Image", corrected_image);
                waitKey(0);
                imwrite("data/corrected_image.jpg", corrected_image);
                destroyWindow("Fully Rectified Image");

            }

        // catch any uncaught exceptions and exit the program
        } catch ( LogMessage l ){
            options.logger.add_message( l );
        } catch ( string e ){
            options.logger.add_message( LOG_MAJOR, e );
        } catch ( ... ){
            options.logger.add_message( LOG_MAJOR, "ERROR: Major issue escaped program control, exiting");
        }

    
    // catch any uncaught exceptions and exit the program
    } catch ( string e ){
        cout << e << endl;
    } catch (...){
        cout << "An unexpected error occurred, leaving now" << endl;
    }

    return 0;
}

