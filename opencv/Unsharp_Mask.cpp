#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <vector>

using namespace cv;

/**
 *  Compute the Unsharp Masked image. 
 *
 * @param[in] input_image  Image to be transformed
 * @param[in/out] output_image Transformed Image.  Will retain same CV Type.
 * @param[in] sigma Strength of Gaussian Blur.
 * @param[in] gauss_window Width of Gaussian Window.
 * @param[in] alpha 
 */
void UnsharpFilter( Mat const& input_image, Mat& output_image, Mat& high_image, Mat& low_image,  double sigma, Size const& gauss_window, double alpha ){

    //split image into channels
    std::vector<Mat> channels, highpass, lowpass;
    split( input_image.clone(), channels );

    highpass.resize( channels.size());
    lowpass.resize(  channels.size());

    //iterate over each channel
    Mat gaussian, diff;
    for( size_t i=0; i<channels.size(); i++ ){

        //compute gaussian kernel
        GaussianBlur( channels[i], gaussian, gauss_window, sigma);

        //subtract images
        diff = channels[i] - gaussian;
        
        //add images back in
        lowpass[i]  = gaussian.clone();
        highpass[i] = diff*10;
        channels[i] = channels[i] + alpha * diff;
    }

    //merge channels together
    Mat merged_image, merged_highp, merged_lowp;
    merge( channels, merged_image );
    merge( highpass, merged_highp );
    merge( lowpass,  merged_lowp  );

    //return image to uchar
    merged_image.convertTo( output_image, input_image.type());
    merged_highp.convertTo( high_image  , input_image.type());
    merged_lowp.convertTo ( low_image,    input_image.type());
}

/**
 * Main Driver
 */
int main( int argc, char* argv[] ){

    //simple error checking
    if( argc < 2 ){
        std::cout << "usage:  ./Unsharp  <input image> <output image>" << std::endl;
        return 0;
    }

    //some relevant variables
    double sigma = 1;
    int    width = 5*sigma;
    double alpha = 4;

    Mat image = imread( argv[1] );

    Mat imageout, highimage, lowimage;
    UnsharpFilter( image, imageout, highimage, lowimage, sigma, Size(width,width), alpha );

    //write result to file
    imwrite("out.png", imageout);
    imwrite("high.png", highimage);
    imwrite("low.png", lowimage);

    return 0;
}
