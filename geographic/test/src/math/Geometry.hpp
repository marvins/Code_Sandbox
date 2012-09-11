#ifndef __SRC_MATH_GEOMETRY_HPP__
#define __SRC_MATH_GEOMETRY_HPP__

#include <opencv2/core/core.hpp>

/** 
  * Compute the intersection between a line and a plane using 2 point for the line and a point for the plane as well as its normal. 
  *
  * @note Be aware that the line points do not need to be in any particular order.
  *
  * @param[in] Line_P1 First point on the line
  * @param[in] Line_P2 Second point on the line
  * @param[in] N Normal vector of the plane
  * @param[in] Plane_P1 Point on the plane
  * @param[in] 4x1 point corresponding to the collision point.
*/
cv::Mat compute_plane_line_intersection( cv::Mat const& Line_P1, cv::Mat const& Line_P2, cv::Mat const& N, cv::Mat const& Plane_P1);

cv::Mat compute_image2projected_coordinate( const int& xx, const int& yy );

#endif
