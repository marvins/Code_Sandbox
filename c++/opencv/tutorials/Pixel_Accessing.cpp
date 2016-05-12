/**
 * @file   Pixel_Accessing.cpp
 * @author Marvin Smith
 * @date   5/12/2016
 */

// OpenCV Libraries
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// C++ Libraries
#include <iostream>


//using namespace cv;


/**
 * @brief Start of Main Application
 */
int main( int argc, char* argv[] )
{

    //read image in color
    cv::Mat color_img = cv::imread( argv[1] );

    //read image in grayscale, NOTE 0 at end
    cv::Mat gray_img  = cv::imread( argv[1], 0 );


    //create display windows
    cv::namedWindow("Fixed Ratio");   //window size is fixed
    cv::namedWindow("Free Ratio", 0); //window may be resized

    //display images
    cv::imshow("Fixed Ratio", color_img);
    cv::imshow("Free Ratio", gray_img);

    //hold program for keyboard input
    cv::waitKey(0);

    // Iterate over grayscale pixels, inverting
    for( int r=0; r<gray_img.rows; r++ )
    for( int c=0; c<gray_img.cols; c++ ){
        gray_img.at<uchar>(r,c) = 255 - gray_img.at<uchar>(r,c);
    }

    
    // Iterate over color pixels, inverting
    for( int r=0; r<color_img.rows; r++ )
    for( int c=0; c<color_img.cols; c++ ){
        color_img.at<cv::Vec3b>(r,c) = cv::Vec3b(255,255,255) - color_img.at<cv::Vec3b>(r,c);
    }

    
    // HERE is another valid way of touching pixels using the channels
    // Iterate over color pixels, inverting
    for( int r=0; r<color_img.rows; r++ )
    for( int c=0; c<color_img.cols; c++ )
    for( int x=0; x<color_img.channels(); x++ ){
        color_img.at<uchar>(r,c)[x] = 255 - color_img.at<uchar>(r,c)[x];
    }


    //display images
    cv::imshow("Fixed Ratio", color_img);
    cv::imshow("Free Ratio", gray_img);

    //hold program for keyboard input
    cv::waitKey(0);

    return 0;
}
