#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <sstream>
#include <vector>

using namespace cv;

double alpha = 4;
double sigma = 1.44;
int    width = 5;
Mat imageout;
Mat image;

void on_trackbar( int, void* ){

    UnsharpFilter( image, imageout, sigma, Size(width,width), alpha);
    imshow("Sharpening Results");

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
void UnsharpFilter( Mat const& input_image, Mat& output_image, double sigma, Size const& gauss_window, double alpha ){

    //split image into channels
    std::vector<Mat> channels;
    split( input_image, channels );

    //iterate over each channel
    Mat gaussian, diff;
    for( size_t i=0; i<channels.size(); i++ ){

        //compute gaussian kernel
        GaussianBlur( channels[i], gaussian, gauss_window, sigma);

        //subtract images
        diff = channels[i] - gaussian;
        
        //add images back in
        channels[i] = channels[i] + alpha * diff;
    }

    //merge channels together
    Mat merged_image;
    merge( channels, merged_image );

    //return image to uchar
    merged_image.convertTo( output_image, input_image.type());
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
    namedWindow("Sharpening Results");

    //read image
    image = imread( argv[1] );

    //apply Unsharp Mask Filter
    UnsharpFilter( image, imageout, sigma, Size(width,width), alpha );
    
    //create trackbar
    createTrackbar( "Alpha", "Sharpening Results", &alpha, 10, 0, on_trackbar);
    createTrackbar( "Sigma", "Sharpening Results", &sigma,  5, 0, on_trackbar);
    createTrackbar( "Width", "Sharpening Results", &width,  11,1, on_trackbar);

    //show stuff
    on_trackbar( alpha, 0);
    on_trackbar( sigma, 0);
    on_trackbar( width, 0);

    //write result to file
    imwrite(argv[2], imageout);

    return 0;
}
