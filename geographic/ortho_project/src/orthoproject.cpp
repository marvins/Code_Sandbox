/**
 * Orthoprojection Test Program
 *
 * @File    src/orthoproject.cpp
 * @Author  Marvin Smith, Sierra Nevada Corporation
 * @Date    21 September 2012
*/

//STL Libraries
#include <iostream>

//OpenCV Libraries
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//Personal Libraries
#include "core/Configuration.hpp"
#include "core/OrthoExceptions.hpp"
#include "ortho/Orthorectify.hpp"
#include "ortho/Perspective.hpp"

//Relevant Namespaces
using namespace cv;
using namespace std;



/**
 * Main Driver
 * 
 * @argc Number of command-line arguments
 * @argv List of commands
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
            
            /**************************/
            /*     BUILD run_type     */
            /**************************/
            if( options.get_run_type() == "FULL" || options.get_run_type() == "BUILD" ){

                //generate our perspective image
                options.image = generate_perspective_test_image( options );
                
                //write and display image
                if(( options.logger.get_console_run_state() & LOG_INFO ) == LOG_INFO ){
                    namedWindow("Modified Image");
                    imshow("Modified Image", options.image);
                    waitKey(0);
                    destroyWindow("Modified Image");
                }
                
                imwrite("data/modified_image.jpg", options.image);
                exit(0);

            }
            
            /******************************************************/
            /*   If we have a FULL run type, convert the image    */
            /******************************************************/
            if( options.get_run_type() == "FULL" ){
                
                //set the dem parameters here
                throw string("Not implemented yet");
            
            }
            /*********************************************/
            /*     Otherwise, load the proper image      */
            /*********************************************/
            else{
                
                /*  Since we are starting from scratch, we need to */
                /*  load the appropriate information.              */
                
                //load image
                // TODO Check if image is a nitf, if so, load with GeoImage
                //      Otherwise, load with opencv
                // TODO Check the color type and bit depth
                options.image = imread( options.image_filename );
                
                //load dem
                // TODO Check if dem is for a geo image or from a file
                // TODO Load from Geo Image DEM Module or from opencv
                options.dem   = imread( options.dem_filename, 0 );

            }

            /*****************************/
            /*      RECTIFY run_type     */
            /*****************************/
            if( options.get_run_type() == "FULL" || options.get_run_type() == "RECTIFY" ){
                
                //rectify the image
                Mat corrected_image = orthorectify( options.image, options );  

                //write and display image
                if(( options.logger.get_console_run_state() & LOG_INFO ) == LOG_INFO ){
                    namedWindow("Fully Rectified Image");
                    imshow("Fully Rectified Image", corrected_image);
                    waitKey(0);
                    destroyWindow("Fully Rectified Image");
                }
                imwrite(options.rectify_output_filename.c_str(), corrected_image);

            }

        // catch any uncaught exceptions and exit the program
        } catch ( LogMessage l ){
            options.logger.add_message( l );
        } catch ( string e ){
            options.logger.add_message( LOG_MAJOR, e );
        } catch ( OrthoException& e ){
            options.logger.add_message( LOG_MAJOR, e.getMessage() );
        } catch ( ... ){
            options.logger.add_message( LOG_MAJOR, "ERROR: Major issue escaped program control, exiting");
        }

    
    // catch any uncaught exceptions and exit the program
    } catch ( string e ){
        cout << e << endl;
    } catch ( OrthoException& e ){
        cout << e.getMessage() << endl;
    } catch (...){
        cout << "An unexpected error occurred, leaving now" << endl;
    }

    return 0;
}

