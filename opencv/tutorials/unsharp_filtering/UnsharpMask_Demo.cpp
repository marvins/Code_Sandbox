#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <sstream>
#include <vector>

using namespace cv;

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
    if( argc < 5 ){
        std::cout << "usage:  ./Unsharp  <input image> <output image> <sigma> <alpha>" << std::endl;
        return 0;
    }

    //load sigma
    double sigma = 1;
    std::stringstream sin;
    sin >> argv[3];
    sin << sigma;
    sin.clear();
    sin.str("");

    //load width
    int    width = 5*sigma;
    
    //load alpha
    double alpha = 9;
    sin >> argv[4];
    sin << alpha;
    sin.clear();
    sin.str("");

    //read image
    Mat image = imread( argv[1] );

    //apply Unsharp Mask Filter
    Mat imageout;
    UnsharpFilter( image, imageout, sigma, Size(width,width), alpha );

    //write result to file
    imwrite(argv[2], imageout);

    return 0;
}
