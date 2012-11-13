#ifndef __SRC_UTILITITES_OPENCV_UTILITIES_HPP__
#define __SRC_UTILITITES_OPENCV_UTILITIES_HPP__


#ifdef DEPLOYED
#include <geoimage/io/IO_Configuration.hpp>
#else
#include "../io/IO_Configuration.hpp"
#endif

#if USE_OPENCV == 1
#include <opencv2/core/core.hpp>

namespace GEO{

/**
 * Convert an OpenCV Type 2 Depth
*/
int cvType2Depth( const int& type );

/**
 * Convert an OpenCV Depth and Channel to OpenCV Type
*/
int cvDepthChannel2Type( const int Depth, const int Channels );

/**
 * Convert an OpenCV Type into a string
*/
std::string opencvType2string( const int& type  );

/**
 * Convert an OpenCV Depth into a string
*/
std::string opencvDepth2string( const int& depth );

/**
 * Set the pixel value
*/
void cvSetPixel( cv::Mat& image, cv::Point const& pix, double const& val );

/**
 * Get the pixel value
*/
double cvGetPixel( cv::Mat const& image, cv::Point const&  pix, const int& channel );

} // End of GEO Namespace
#endif

#endif

