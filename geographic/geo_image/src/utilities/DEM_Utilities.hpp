#ifndef __SRC_UTILITIES_DEM_UTILITIES_HPP__
#define __SRC_UTILITIES_DEM_UTILITIES_HPP__


#ifdef DEPLOYED
#include <geoimage/io/IO_Configuration.hpp>
#else
#include "../io/IO_Configuration.hpp"
#endif

#if USE_OPENCV == 1
#include <opencv2/core/core.hpp>
#endif

#include <string> 

namespace GEO{

std::string DTED_coordinate2filename( double const& lat, double const& lon );

#if USE_OPENCV == 1
void DTED_adjust_needed_tiles( int& lat_needed, int& lon_needed, const cv::Point2f& _min, const cv::Point2f& _max );
#endif

}

#endif

