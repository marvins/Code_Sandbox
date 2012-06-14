///OpenCV Headers
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

///STL Headers
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

///Misc. Flags
bool quit  = false;
int  mode  = 0;
int  alpha = 4;
int  sigma = 1.44;
int  width = 5;
int  w_act = 5;
int  thresh= 100;

///Image list
vector<Mat> image_list;

///Unsharp prototype
void UnsharpFilter( );

/**
 * Trackbar function for image width
*/
void on_trackbar( int, void* ){
    
    /** Make sure the width is never even */
    if( w_act % 2 == 0 ) width = w_act + 1;
    else                 width = w_act;

    UnsharpFilter(  );
}

/**
 * Trackbar function for threshold
*/
void thresh_trackbar( int, void* ){
    UnsharpFilter(  );
}

/**
 * Change the type of image to show
*/
void modeChange( int state, void*  foo ){
    
    mode++;
    if( mode > 2 ) mode = 0;
    imshow("Sharpening Results", image_list[mode]);
}

/**
 * Exit the program
*/
void quitProgram( int state, void* foo ){
    exit(0);
}

/**
 * Write the current image to file
*/
void writeImage( int state, void* foo ){
    imwrite( "output.jpg", image_list[mode] );
}


/**
 *  Compute the Unsharp Masked image. 
 *
 * @param[in] input_image  Image to be transformed
 * @param[in/out] output_image Transformed Image.  Will retain same CV Type.
 * @param[in] sigma Strength of Gaussian Blur.
 * @param[in] gauss_window Width of Gaussian Window.
 * @param[in] alpha Scale factor for high-pass image
 */
void UnsharpFilter(  ){

    //split image into channels
    std::vector<Mat> channels, diff_list;
    split( image_list[0], channels );
    diff_list.resize(channels.size());

    //iterate over each channel
    Mat gaussian;
    for( size_t i=0; i<channels.size(); i++ ){

        //compute gaussian kernel
        GaussianBlur( channels[i], gaussian, Size( width, width), sigma);

        //subtract images
        diff_list[i]= channels[i] - gaussian;
        
        //add images back in
        channels[i] = channels[i] + alpha * diff_list[i];
    }

    //merge channels together
    Mat merged_image;
    merge(  channels, merged_image );
    merge( diff_list, image_list[2]);

    //return image to uchar
    merged_image.convertTo( image_list[1], image_list[0].type());
}

/**
 * Main Driver
 */
int main( int argc, char* argv[] ){

    //simple error checking
    if( argc < 2 ){
        std::cout << "usage:  ./Unsharp  <input image>" << std::endl;
        return 0;
    }
    
    //create window
    namedWindow("Sharpening Results", CV_WINDOW_AUTOSIZE );

    //create trackbar
    createTrackbar( "Alpha" , "Sharpening Results",  &alpha,  10, on_trackbar);
    createTrackbar( "Sigma" , "Sharpening Results",  &sigma,   5, on_trackbar);
    createTrackbar( "Width" , "Sharpening Results",  &w_act,  11, on_trackbar);
    createTrackbar( "Thresh", "Sharpening Results", &thresh, 255, thresh_trackbar);

    //create buttons
    cvCreateButton( "Change Image",  modeChange, NULL, CV_PUSH_BUTTON );
    cvCreateButton( "Quit Program", quitProgram, NULL, CV_PUSH_BUTTON );
    cvCreateButton( "Write Image" , writeImage , NULL, CV_PUSH_BUTTON );

    //read image
    image_list.resize(3);
    image_list[0] = imread( argv[1], 0 );

    //apply Unsharp Mask Filter
    UnsharpFilter(  );
    
    //show stuff
    on_trackbar( alpha, 0);
    on_trackbar( sigma, 0);
    on_trackbar( width, 0);
   
    imshow("Sharpening Results", image_list[0]);
    char key;
    while( quit == false && key != 'q' ){
        key = waitKey(0);
    }
        
    return 0;
}
