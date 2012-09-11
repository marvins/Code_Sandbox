#ifndef __SRC_CORE_UTILITIES_HPP__
#define __SRC_CORE_UTILITIES_HPP__

#include <opencv2/core/core.hpp>

/**
  * A simple function for converting 3 values into a 4x1 Homogenous coordinate in cv::Mat format
  *
  * @param[in] x X Parameter
  * @param[in] y Y Parameter
  * @param[in] z Z Parameter
  * @return A 4x1 Matrix in the form [[x],[y],[z],[1]]
*/
cv::Mat load_point(  const double& x, const double& y, const double& z );

/**
  * A simple function for converting 3 values into a 4x1 Homogenous Vector in cv::Mat format
  *
  * @param[in] x X Parameter
  * @param[in] y Y Parameter
  * @param[in] z Z Parameter
  * @return A 4x1 Matrix in the form [[x],[y],[z],[0]]
*/
cv::Mat load_vector( const double& x, const double& y, const double& z );

/**
  * Print an OpenCV Matrix to the console in a pretty format
  *
  * @param[in] mat Matrix to print
*/
void print_mat( const cv::Mat& mat );


#endif

