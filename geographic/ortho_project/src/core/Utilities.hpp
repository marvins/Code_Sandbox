#ifndef __SRC_CORE_UTILITIES_HPP__
#define __SRC_CORE_UTILITIES_HPP__

#include <opencv2/core/core.hpp>


/**
 * A string to value conversion using stringstreams
 * 
 * @param[in] value String to convert
 * @return Converted number
*/
template <typename TP>
TP str2num( std::string const& value ){
    
    std::stringstream sin;
    TP result;
    sin << value;
    sin >> result;
    return result;
}

template <typename TP>
std::string num2str( TP const& value ){

    std::stringstream sin;
    sin << value;
    return sin.str();
}


/** 
 * A function for converting a 4x1 or 3x1 Matrix into an OpenCV Point3f
*/
cv::Point3f Mat2Point3f( cv::Mat const& mat );

/** 
  * A simple function for converting an OpenCV point into a 4x1 Homogenous coordinate in cv::Mat format
  *
  * @param[in] pnt OpenCV Point
  * @return A 4x1 Matrix in the form [[x],[y],[z],[1]]
*/
cv::Mat load_point( const cv::Point3f& pnt );

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

/**
 * Round a double to the nearest integer
 *
 * @param[in] value Double to be rounded
 * @return integer of the rounded value
*/
int _round( const double& value );

#endif

