#ifndef __SRC_UTILITIES_DEM_UTILITIES_HPP__
#define __SRC_UTILITIES_DEM_UTILITIES_HPP__

#include <opencv2/core/core.hpp>

#include <string> 

namespace GEO{

std::string DTED_coordinate2filename( double const& lat, double const& lon );

void DTED_adjust_needed_tiles( int& lat_needed, int& lon_needed, const cv::Point2f& _min, const cv::Point2f& _max );

}

#endif

