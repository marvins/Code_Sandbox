/**
 * @file    stabilization-demo.cpp
 * @author  Marvin Smith
 * @date    1/21/2019
 */

// C++ Libraries
#include <iostream>
#include <string>
#include <vector>


// Project Libraries
#include "Frame_Loader.hpp"
#include "Log_Utilities.hpp"
#include "Options.hpp"

// OpenCV Libraries
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>


int main( int argc, char* argv[] )
{

    try{

        // Build Configuration
        Options options( argc, argv );

        // Build the frame-loader
        auto frame_loader = std::make_shared<Frame_Loader>(options);
        frame_loader->Initialize();

        // Create the feature detector
        //auto detector = cv::GFTTDetector::create();
        auto detector = cv::ORB::create();

        auto frame_number_set = frame_loader->Get_Frame_Number_Set();
        for( auto frame_number : frame_number_set )
        {
            // Get the image
            auto frame_info = frame_loader->Get_Loaded_Frame_Info( frame_number );

            if( frame_info == nullptr ){
                LOG_ERROR("Unable to find info for frame " + std::to_string(frame_number));
                continue;
            }

            // Detect Features
            std::vector<cv::KeyPoint> keypoints;
            cv::Mat descriptors;
            detector->detectAndCompute( frame_info->Get_Image(),
                                        cv::noArray(),
                                        keypoints,
                                        descriptors );
            LOG_INFO("Found " + std::to_string(keypoints.size()) + " keypoints");
            LOG_INFO("Descriptor Size: " + std::to_string(descriptors.rows) + " rows, "
                     + std::to_string(descriptors.cols) + " cols");

            // Match Features
            cv::Mat match_image;
            cv::drawKeypoints( frame_info->Get_Image(), keypoints, match_image);

            // Compute new stabilization transform

            // Apply Transform

            // Show Results
            cv::imshow("Input Image", match_image);
            cv::waitKey(100);
        }

        frame_loader->Finalize();
    }
    catch( std::exception& e )
    {
        std::cerr << "Exception Caught: " << e.what() << std::endl;
        return 1;
    }
    catch( ... )
    {
        std::cerr << "Unknown exception caught." << std::endl;
    }

    return 0;
}