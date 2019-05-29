/**
 * @file    Optical_Flow.cpp
 */
#include "Optical_Flow.hpp"

// OpenCV Libraries
#include <opencv2/video.hpp>

// Project Libraries
#include "Ring_Buffer.hpp"


/********************************************************************/
/*          Run Sparse Pyramid Lukas Kanade Optical FLow            */
/********************************************************************/
void SparsePyrLkOptFlowEstimator::run( cv::InputArray frame0,
                                       cv::InputArray frame1,
                                       cv::InputArray points0,
                                       cv::InputOutputArray points1,
                                       cv::OutputArray status,
                                       cv::OutputArray errors)
{
    calcOpticalFlowPyrLK( frame0,
                          frame1,
                          points0,
                          points1,
                          status,
                          errors,
                          winSize_,
                          maxLevel_);
}
