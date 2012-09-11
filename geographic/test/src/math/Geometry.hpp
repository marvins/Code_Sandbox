#ifndef __SRC_MATH_GEOMETRY_HPP__
#define __SRC_MATH_GEOMETRY_HPP__

#include <opencv2/core/core.hpp>

#include <vector>

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

/**
  * Convert the coordinate in the pixel space into the world coordinate system.
  *
  * @param[in] pnt Point in the image space.
  * @param[in] RotationMatrix A rotation matrix to rotate the camera with
  * @param[in] camera_origin The origin of the Camera in the world space
  * @param[in] image2camera the transformation from image space to camera space
  * @return the geographic location of the camera point in world coordinates
*/
cv::Mat pixel2world_coordinates( const cv::Point3f& pnt, const cv::Mat& RotationMatrix, const cv::Mat& camera_origin, const cv::Mat& image2camera );

/**
  * Add a translation to a 4x4 matrix
  *
  * @param[out] matrix Input matrix
  * @param[in] translation Translation value
*/
void matrix_add_translation( cv::Mat& matrix, cv::Mat& translation );

std::vector<std::vector<cv::Point3f> > build_ground_coordinate_list( cv::Mat const& dem, cv::Size img_size, const double& f, const cv::Mat& RotationM, const cv::Mat& camera_origin, const cv::Mat& img2cam );  


double compute2d_line_point_distance( cv::Point3f const& l1, cv::Point3f const& l2, cv::Point3f const& pt );
double compute3d_line_point_distance( cv::Point3f const& l1, cv::Point3f const& l2, cv::Point3f const& pt );

/**
 * 0 - No intersection, return the distance between lines
 * 1 - Intersection of Line A occured on an endpoint of line B
 * 2 - Intersection of Line A occured on the interior of line B
*/
int compute3d_line_line_intersection( cv::Point3f const a1, cv::Point3f const a2, cv::Point3f const& b1, cv::Point3f const& b2, double& distance, double const& threshold );

#endif
