/**
 * @file    analysis-utility.cpp
 * @author  Marvin Smith
 * @date    2/11/2016
*/

// OpenCV Libraries
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

// C++ Libraries
#include <iostream>
#include <string>
#include <vector>


/**
 * @brief main
*/
int main( int argc, char* argv[] )
{

    // Load the Reference Image
    std::string ref_image_path = argv[1];
    std::string tst_image_path = argv[2];


    // Load the Images
    cv::Mat tst_image = cv::imread( tst_image_path.c_str(), 0);
    cv::Mat ref_image = cv::imread( ref_image_path.c_str(), 0);

    // Exit Application
    return 0;
}

