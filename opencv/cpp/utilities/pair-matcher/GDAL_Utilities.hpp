/**
 * @file    GDAL_Utilities.hpp
 * @author  Marvin Smith
 * @date    2/18/2016
 */
#ifndef __PAIR_MATCHER_GDAL_UTILITIES_HPP__
#define __PAIR_MATCHER_GDAL_UTILITIES_HPP__

// OpenCV Libraries
#include <opencv2/core.hpp>

/**
 * @brief Load image from GDAL to OpenCV
 */
cv::Mat  Load_Image_GDAL_To_OpenCV( const std::string& pathname,
                                    double*&           geo_transform );


#endif
