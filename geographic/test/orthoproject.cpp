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
#include "src/core/Configuration.hpp"
#include "src/ortho/Orthorectify.hpp"
#include "src/ortho/Perspective.hpp"

//Relevant Namespaces
using namespace cv;
using namespace std;



/**
 * Main Driver
*/
int main( int argc, char* argv[] ){

    try{

        // Load and open the configuration file
        Options options(argc, argv);
        
        //print configuration to the screen
        options.print();
        
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




    } catch ( string e ){
        cout << e << endl;
    } catch (...){
        cout << "An unexpected error occurred, leaving now" << endl;
    }

    return 0;
}

