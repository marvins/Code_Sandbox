/**
 * @file    analysis-utility.cpp
 * @author  Marvin Smith
 * @date    2/11/2016
*/

// OpenCV Libraries
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>


// C++ Libraries
#include <iostream>
#include <string>
#include <vector>


// Analysis Utility Libraries
#include "Options.hpp"


/**
 * @brief main
*/
int main( int argc, char* argv[] )
{
    
    // Parse Command-Line Options
    Options options(argc, argv);


    // Load Images
    cv::Mat ref_image = cv::imread( options.Get_Ref_Image_Path().c_str(), 0 );
    cv::Mat tst_image = cv::imread( options.Get_Test_Image_Path().c_str(), 0 );

    // View Images
    cv::imshow("Ref Image", ref_image);
    cv::imshow("Test Image", tst_image);
    cv::waitKey(0);
    
    // Exit Application
    return 0;
}

