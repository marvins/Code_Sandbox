#ifndef __SRC_MATH_GEOMETRY_HPP__
#define __SRC_MATH_GEOMETRY_HPP__

#include <opencv2/core/core.hpp>

cv::Mat compute_plane_line_intersection( cv::Mat const& P1, cv::Mat const& P2, cv::Mat const& N, cv::Mat const& P3);

cv::Mat compute_image2projected_coordinate( const int& xx, const int& yy );

#endif
